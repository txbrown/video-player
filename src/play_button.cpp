#include "play_button.h"
#include <wx/dcbuffer.h>
#include "include/core/SkCanvas.h"
#include "include/core/SkPath.h"
#include "include/core/SkPaint.h"
#include "utils/skia_utils.h"
IMPLEMENT_DYNAMIC_CLASS(PlayButton, wxControl);
BEGIN_EVENT_TABLE(PlayButton, wxControl)
EVT_PAINT(PlayButton::OnPaint)
EVT_LEFT_DOWN(PlayButton::OnMouseDown)
EVT_LEFT_UP(PlayButton::OnMouseUp)
EVT_SIZE(PlayButton::OnSize)
END_EVENT_TABLE()

PlayButton::PlayButton()
{
    Init();
}

PlayButton::PlayButton(wxWindow *parent,
                       wxWindowID winid,
                       const wxString &label,
                       const wxPoint &pos,
                       const wxSize &size,
                       long style,
                       const wxValidator &val,
                       const wxString &name)
{
    Init();
    Create(parent, winid, label, pos, size, style, val, name);
}

bool PlayButton::Create(wxWindow *parent,
                        wxWindowID winid,
                        const wxString &label,
                        const wxPoint &pos,
                        const wxSize &size,
                        long style,
                        const wxValidator &val,
                        const wxString &name)
{
    if (!wxControl::Create(parent, winid, pos, size, wxBORDER_NONE | wxWANTS_CHARS, val, name))
        return false;

    wxSize buttonSize(32, 32);
    SetSize(buttonSize);
    SetMinSize(buttonSize);
    SetMaxSize(buttonSize);

    InitSkia();
    return true;
}

void PlayButton::Init()
{
    m_surface = nullptr;
    m_isHovered = false;
    m_isPressed = false;
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void PlayButton::OnPaint(wxPaintEvent &)
{
    if (!m_surface)
    {
        InitSkia();
    }

    if (!m_surface)
    {
        return;
    }

    auto canvas = m_surface->getCanvas();
    RenderButton(canvas);

    wxBitmap bitmap = utils::SkiaSurfaceToWxBitmap(m_surface);
    if (bitmap.IsOk())
    {
        wxPaintDC dc(this);
        dc.DrawBitmap(bitmap, 0, 0, true);
    }
}

void PlayButton::InitSkia()
{
    int width, height;
    GetSize(&width, &height);

    width = std::max(width, 1);
    height = std::max(height, 1);

    SkImageInfo info = SkImageInfo::Make(
        width, height,
        kRGB_888x_SkColorType,
        kPremul_SkAlphaType);
    m_surface = SkSurfaces::Raster(info);
}

void PlayButton::OnMouseEnter(wxMouseEvent &evt)
{
    m_isHovered = true;
    Refresh();
    evt.Skip();
}

void PlayButton::OnMouseLeave(wxMouseEvent &evt)
{
    m_isHovered = false;
    Refresh();
    evt.Skip();
}

void PlayButton::OnMouseDown(wxMouseEvent &evt)
{
    m_isPressed = true;
    Refresh();
    evt.Skip();
}

void PlayButton::OnMouseUp(wxMouseEvent &evt)
{
    m_isPressed = false;
    if (m_pressCallback)
    {
        m_pressCallback();
    }
    Refresh();
    evt.Skip();
}

void PlayButton::RenderButton(SkCanvas *canvas)
{
    canvas->clear(SK_ColorTRANSPARENT);

    wxSize size = GetSize();
    SkPaint paint;

    if (m_isPressed)
    {
        paint.setColor(Colors::DARK_GRAY);
    }
    else if (m_isHovered)
    {
        paint.setColor(Colors::LIGHT_GRAY);
    }
    else
    {
        paint.setColor(Colors::WHITE);
    }

    paint.setStrokeWidth(2);
    paint.setAntiAlias(true);

    const float centerX = size.x / 2.0f;
    const float centerY = size.y / 2.0f;

    const float arrowHeight = size.y;
    const float arrowWidth = arrowHeight * 0.8f;
    const float spacing = arrowWidth * 0.3f;

    SkPath path;
    path.moveTo(centerX - spacing, centerY - arrowHeight / 2);
    path.lineTo(centerX - spacing + arrowWidth, centerY);
    path.lineTo(centerX - spacing, centerY + arrowHeight / 2);

    path.close();

    canvas->drawPath(path, paint);
}

void PlayButton::OnSize(wxSizeEvent &evt)
{
    m_surface.reset();
    Refresh();
    evt.Skip();
}

wxSize PlayButton::DoGetBestSize() const
{
    return wxSize(32, 32);
}

void PlayButton::OnPress(std::function<void()> callback)
{
    m_pressCallback = callback;
}