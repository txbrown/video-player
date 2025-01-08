#pragma once

#include <wx/wx.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkSurface.h>
#include <include/core/SkPaint.h>
#include <vector>
#include "video_state.h"
#include "common.h"

class ProgressBar : public wxPanel
{
public:
  ProgressBar(wxWindow *parent);
  virtual ~ProgressBar() = default;

  void SetProgress(double progress);         // 0.0 to 1.0
  void SetDuration(double duration);         // in seconds
  void SetBufferedProgress(double buffered); // 0.0 to 1.0
  void AddChapter(const Chapter &chapter);
  void ClearChapters();
  void SetLoading(bool isLoading);

  // Event handlers
  void OnSeek(const std::function<void(double)> &callback);
  void OnBackward(const std::function<void()> &callback);
  void OnForward(const std::function<void()> &callback);
  void OnTogglePlay(const std::function<void()> &callback);

private:
  // UI Events
  void OnPaint(wxPaintEvent &evt);
  void OnMouseDown(wxMouseEvent &evt);
  void OnMouseUp(wxMouseEvent &evt);
  void OnMouseMove(wxMouseEvent &evt);
  void OnKeyDown(wxKeyEvent &evt);
  void OnTimer(wxTimerEvent &evt);
  void OnSize(wxSizeEvent &evt);

  // Rendering
  void InitSkia();
  void RenderProgressBar(SkCanvas *canvas);
  void RenderChapters(SkCanvas *canvas);
  void RenderLoadingAnimation(SkCanvas *canvas);
  void UpdateLoadingAnimation();
  void RenderBackwardButton(SkCanvas *canvas);
  void RenderForwardButton(SkCanvas *canvas);
  void RenderPlayButton(SkCanvas *canvas);
  // Helper methods
  double PixelsToProgress(int x) const;
  int ProgressToPixels(double progress) const;
  Chapter *GetChapterAtProgress(double progress);

  // State
  sk_sp<SkSurface> m_surface;
  std::vector<Chapter> m_chapters;
  double m_progress{0.0};
  double m_bufferedProgress{0.0};
  double m_duration{0.0};
  bool m_isLoading{false};
  bool m_isDragging{false};
  wxPoint m_dragStart;
  double m_dragStartProgress{0.0};

  // Loading animation
  wxTimer m_loadingTimer;
  float m_loadingAngle{0.0f};

  // Callback
  std::function<void(double)> m_seekCallback;

  DECLARE_EVENT_TABLE()
};