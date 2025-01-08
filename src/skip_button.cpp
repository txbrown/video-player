#include "skip_button.h"
#include "utils/skia_utils.h"
#include "common.h"

IMPLEMENT_DYNAMIC_CLASS(SkipButton, wxControl);
BEGIN_EVENT_TABLE(SkipButton, wxControl)
EVT_PAINT(SkipButton::OnPaint)
EVT_LEFT_DOWN(SkipButton::OnMouseDown)
EVT_LEFT_UP(SkipButton::OnMouseUp)
EVT_SIZE(SkipButton::OnSize)
END_EVENT_TABLE()

SkipButton::SkipButton(SkipDirection direction)
{
    Init(direction);
}

SkipButton::SkipButton(wxWindow *parent,
                       SkipDirection direction,
                       wxWindowID winid,
                       const wxString &label,
                       const wxPoint &pos,
                       const wxSize &size,
                       long style,
                       const wxValidator &val,
                       const wxString &name)
{
    Init(direction);
    Create(parent, direction, winid, label, pos, size, style, val, name);
}

void SkipButton::Init(SkipDirection direction)
{
    m_surface = nullptr;
    m_isHovered = false;
    m_isPressed = false;
    m_direction = direction;
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

bool SkipButton::Create(wxWindow *parent,
                        SkipDirection direction,
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

void SkipButton::InitSkia()
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

wxSize SkipButton::DoGetBestSize() const
{
    return wxSize(88, 88);
}

void SkipButton::OnPaint(wxPaintEvent &)
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

void SkipButton::RenderButton(SkCanvas *canvas)
{
    // Clear background
    canvas->clear(SK_ColorTRANSPARENT);

    wxSize size = GetSize();
    SkPaint paint;

    // Adjust color based on state
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

    // Adjust these values to control arrow size and spacing
    const float arrowHeight = size.y * 0.4f;     // Height of each arrow
    const float arrowWidth = arrowHeight * 0.8f; // Width of each arrow
    const float spacing = arrowWidth * 0.3f;     // Space between arrows

    SkPath arrow1, arrow2;

    if (m_direction == FORWARD)
    {
        // First arrow (left)
        arrow1.moveTo(centerX - spacing, centerY - arrowHeight / 2); // Top
        arrow1.lineTo(centerX - spacing + arrowWidth, centerY);      // Point
        arrow1.lineTo(centerX - spacing, centerY + arrowHeight / 2); // Bottom

        // Second arrow (right)
        arrow2.moveTo(centerX + spacing, centerY - arrowHeight / 2); // Top
        arrow2.lineTo(centerX + spacing + arrowWidth, centerY);      // Point
        arrow2.lineTo(centerX + spacing, centerY + arrowHeight / 2); // Bottom
    }
    else
    {
        // First arrow (right)
        arrow1.moveTo(centerX + spacing + arrowWidth, centerY - arrowHeight / 2); // Top
        arrow1.lineTo(centerX + spacing, centerY);                                // Point
        arrow1.lineTo(centerX + spacing + arrowWidth, centerY + arrowHeight / 2); // Bottom

        // Second arrow (left)
        arrow2.moveTo(centerX - spacing + arrowWidth, centerY - arrowHeight / 2); // Top
        arrow2.lineTo(centerX - spacing, centerY);                                // Point
        arrow2.lineTo(centerX - spacing + arrowWidth, centerY + arrowHeight / 2); // Bottom
    }

    arrow1.close();
    arrow2.close();

    canvas->drawPath(arrow1, paint);
    canvas->drawPath(arrow2, paint);
}

void SkipButton::OnMouseEnter(wxMouseEvent &evt)
{
    m_isHovered = true;
    Refresh();
    evt.Skip();
}

void SkipButton::OnMouseLeave(wxMouseEvent &evt)
{
    m_isHovered = false;
    Refresh();
    evt.Skip();
}

void SkipButton::OnMouseDown(wxMouseEvent &evt)
{
    m_isPressed = true;
    Refresh();
    evt.Skip();
}

void SkipButton::OnMouseUp(wxMouseEvent &evt)
{
    if (m_isPressed)
    {
        m_isPressed = false;
        Refresh();

        // Generate button click event
        wxCommandEvent event(wxEVT_BUTTON, GetId());
        event.SetEventObject(this);
        ProcessEvent(event);
    }
    evt.Skip();
}

void SkipButton::OnSize(wxSizeEvent &evt)
{
    m_surface.reset();
    Refresh();
    evt.Skip();
}