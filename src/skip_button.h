#pragma once

#include <memory>
#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <wx/wx.h>

enum SkipDirection
{
    BACKWARD,
    FORWARD
};

enum SkipButtonStyle
{
    SB_DEFAULT_STYLE = 0,
    SB_ROUNDED = 1,
    SB_WITH_LABEL = 2,
    SB_HOVER_EFFECT = 4
};

class SkipButton : public wxControl
{
public:
    SkipButton(SkipDirection direction = FORWARD);
    virtual ~SkipButton() = default;

    SkipButton(wxWindow *parent,
               SkipDirection direction = FORWARD,
               wxWindowID winid = wxID_ANY,
               const wxString &label = wxEmptyString,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = SB_DEFAULT_STYLE,
               const wxValidator &val = wxDefaultValidator,
               const wxString &name = wxASCII_STR("SkipButton"));

    bool Create(wxWindow *parent,
                SkipDirection direction = FORWARD,
                wxWindowID winid = wxID_ANY,
                const wxString &label = wxEmptyString,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = SB_DEFAULT_STYLE,
                const wxValidator &val = wxDefaultValidator,
                const wxString &name = wxASCII_STR("SkipButton"));

    SkipDirection GetDirection() const { return m_direction; }

protected:
    void Init(SkipDirection direction);
    void InitSkia();
    virtual wxSize DoGetBestSize() const wxOVERRIDE;

private:
    sk_sp<SkSurface> m_surface;
    SkipDirection m_direction;
    bool m_isHovered{false};
    bool m_isPressed{false};

    void RenderButton(SkCanvas *canvas);
    void DrawArrows(SkCanvas *canvas, const SkPaint &paint);

    void OnPaint(wxPaintEvent &evt);
    void OnMouseEnter(wxMouseEvent &evt);
    void OnMouseLeave(wxMouseEvent &evt);
    void OnMouseDown(wxMouseEvent &evt);
    void OnMouseUp(wxMouseEvent &evt);
    void OnSize(wxSizeEvent &evt);

    wxDECLARE_DYNAMIC_CLASS(SkipButton);
    wxDECLARE_EVENT_TABLE();
};