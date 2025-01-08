#include "pause_button.h"
#include <wx/dcbuffer.h>
#include "include/core/SkCanvas.h"
#include "include/core/SkPath.h"
#include "include/core/SkPaint.h"
#include "utils/skia_utils.h"
IMPLEMENT_DYNAMIC_CLASS(PauseButton, wxControl);
BEGIN_EVENT_TABLE(PauseButton, wxControl)
EVT_PAINT(PauseButton::OnPaint)
EVT_LEFT_DOWN(PauseButton::OnMouseDown)
EVT_LEFT_UP(PauseButton::OnMouseUp)
EVT_SIZE(PauseButton::OnSize)
END_EVENT_TABLE()

PauseButton::PauseButton()
{
    Init();
}

PauseButton::PauseButton(wxWindow *parent,
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

bool PauseButton::Create(wxWindow *parent,
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

void PauseButton::Init()
{
    m_surface = nullptr;
    m_isHovered = false;
    m_isPressed = false;
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void PauseButton::OnPaint(wxPaintEvent &)
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

void PauseButton::InitSkia()
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

void PauseButton::OnMouseEnter(wxMouseEvent &evt)
{
    m_isHovered = true;
    Refresh();
    evt.Skip();
}

void PauseButton::OnMouseLeave(wxMouseEvent &evt)
{
    m_isHovered = false;
    Refresh();
    evt.Skip();
}

void PauseButton::OnMouseDown(wxMouseEvent &evt)
{
    m_isPressed = true;
    Refresh();
    evt.Skip();
}

void PauseButton::OnMouseUp(wxMouseEvent &evt)
{
    m_isPressed = false;
    if (m_pressCallback)
    {
        m_pressCallback();
    }
    Refresh();
    evt.Skip();
}
void PauseButton::RenderButton(SkCanvas *canvas)
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
    const float barWidth = 4.0f;
    const float barHeight = size.y;
    const float spacing = size.x * 0.2f;
    const float leftBarX = centerX - (spacing + barWidth);
    const float rightBarX = centerX + spacing;
    const float barY = centerY - (barHeight / 2.0f);

    canvas->drawRect(SkRect::MakeXYWH(leftBarX, barY, barWidth, barHeight), paint);
    canvas->drawRect(SkRect::MakeXYWH(rightBarX, barY, barWidth, barHeight), paint);
}

void PauseButton::OnSize(wxSizeEvent &evt)
{
    m_surface.reset();
    Refresh();
    evt.Skip();
}

wxSize PauseButton::DoGetBestSize() const
{
    return wxSize(32, 32);
}

void PauseButton::OnPress(std::function<void()> callback)
{
    m_pressCallback = callback;
}