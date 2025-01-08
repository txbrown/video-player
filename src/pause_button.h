#pragma once

#include <memory>
#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <wx/wx.h>
#include "common.h"

class PauseButton : public wxControl
{
public:
    PauseButton();
    virtual ~PauseButton() = default;

    PauseButton(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxString &label = wxEmptyString,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = DefaultButtonStyle::DEFAULT_STYLE,
                const wxValidator &val = wxDefaultValidator,
                const wxString &name = wxASCII_STR("PauseButton"));

    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxString &label = wxEmptyString,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = DefaultButtonStyle::DEFAULT_STYLE,
                const wxValidator &val = wxDefaultValidator,
                const wxString &name = wxASCII_STR("PauseButton"));

    void OnPress(std::function<void()> callback);

protected:
    void InitSkia();
    virtual wxSize DoGetBestSize() const wxOVERRIDE;

private:
    sk_sp<SkSurface> m_surface;
    bool m_isHovered{false};
    bool m_isPressed{false};
    std::function<void()> m_pressCallback;

    void Init();
    void RenderButton(SkCanvas *canvas);
    void DrawArrows(SkCanvas *canvas, const SkPaint &paint);

    void OnPaint(wxPaintEvent &evt);
    void OnMouseEnter(wxMouseEvent &evt);
    void OnMouseLeave(wxMouseEvent &evt);
    void OnMouseDown(wxMouseEvent &evt);
    void OnMouseUp(wxMouseEvent &evt);
    void OnSize(wxSizeEvent &evt);

    wxDECLARE_DYNAMIC_CLASS(PauseButton);
    wxDECLARE_EVENT_TABLE();
};