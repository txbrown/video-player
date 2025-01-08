#include "progress_bar.h"
#include <include/core/SkPixmap.h>
#include <include/effects/SkGradientShader.h>
#include "utils/skia_utils.h"

BEGIN_EVENT_TABLE(ProgressBar, wxPanel)
EVT_PAINT(ProgressBar::OnPaint)
EVT_LEFT_DOWN(ProgressBar::OnMouseDown)
EVT_LEFT_UP(ProgressBar::OnMouseUp)
EVT_MOTION(ProgressBar::OnMouseMove)
EVT_KEY_DOWN(ProgressBar::OnKeyDown)
EVT_TIMER(wxID_ANY, ProgressBar::OnTimer)
EVT_SIZE(ProgressBar::OnSize)
END_EVENT_TABLE()

ProgressBar::ProgressBar(wxWindow *parent)
    : wxPanel(parent, wxID_ANY),
      m_loadingTimer(this)
{
  SetMinSize(wxSize(-1, 24));
  SetBackgroundColour(*wxBLACK);
}

void ProgressBar::InitSkia()
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

void ProgressBar::OnPaint(wxPaintEvent &evt)
{
  wxPaintDC dc(this);

  if (!m_surface)
  {
    InitSkia();
  }

  if (m_surface)
  {
    SkCanvas *canvas = m_surface->getCanvas();
    canvas->clear(SK_ColorBLACK);

    RenderProgressBar(canvas);
    RenderChapters(canvas);

    if (m_isLoading)
    {
      RenderLoadingAnimation(canvas);
    }

    // Transfer to wxWidgets
    wxBitmap bitmap = utils::SkiaSurfaceToWxBitmap(m_surface);
    if (bitmap.IsOk())
    {
      dc.DrawBitmap(bitmap, 0, 0, true);
    }
  }
}

void ProgressBar::RenderProgressBar(SkCanvas *canvas)
{
  int width = GetSize().x;
  int height = GetSize().y;

  // Background track
  SkPaint trackPaint;
  trackPaint.setColor(SkColorSetRGB(40, 40, 40));
  canvas->drawRect(SkRect::MakeWH(width, height), trackPaint);

  // Buffered progress
  if (m_bufferedProgress > 0)
  {
    SkPaint bufferedPaint;
    bufferedPaint.setColor(SkColorSetRGB(80, 80, 80));
    canvas->drawRect(
        SkRect::MakeWH(width * m_bufferedProgress, height),
        bufferedPaint);
  }

  // Progress
  if (m_progress > 0)
  {
    SkPaint progressPaint;
    progressPaint.setColor(Colors::YOUTUBE_RED);
    canvas->drawRect(
        SkRect::MakeWH(width * m_progress, height),
        progressPaint);
  }
}

void ProgressBar::RenderChapters(SkCanvas *canvas)
{
  int width = GetSize().x;
  int height = GetSize().y;

  SkPaint chapterPaint;
  chapterPaint.setColor(SK_ColorWHITE);
  chapterPaint.setStrokeWidth(2);

  for (const auto &chapter : m_chapters)
  {
    float startX = width * (chapter.startTime / m_duration);
    canvas->drawLine(
        startX, 0,
        startX, height,
        chapterPaint);
  }
}

void ProgressBar::RenderLoadingAnimation(SkCanvas *canvas)
{
  // Create loading animation bars
  const int numBars = 10;
  const float barWidth = GetSize().x / (numBars * 2);
  const float spacing = barWidth;

  SkPaint loadingPaint;
  loadingPaint.setColor(SkColorSetRGB(255, 255, 255));

  for (int i = 0; i < numBars; i++)
  {
    float alpha = fmod(i + m_loadingAngle, (float)numBars) / numBars;
    loadingPaint.setAlpha(alpha * 255);

    float x = i * (barWidth + spacing);
    canvas->drawRect(
        SkRect::MakeXYWH(x, 0, barWidth, GetSize().y),
        loadingPaint);
  }
}

void ProgressBar::OnMouseDown(wxMouseEvent &evt)
{
  m_isDragging = true;
  m_dragStart = evt.GetPosition();
  m_dragStartProgress = m_progress;
  CaptureMouse();
}

void ProgressBar::OnMouseUp(wxMouseEvent &evt)
{
  if (m_isDragging)
  {
    m_isDragging = false;
    ReleaseMouse();

    if (m_seekCallback)
    {
      m_seekCallback(m_progress * m_duration);
    }
  }
}

void ProgressBar::OnMouseMove(wxMouseEvent &evt)
{
  if (m_isDragging)
  {
    int deltaX = evt.GetPosition().x - m_dragStart.x;
    double progressDelta = (double)deltaX / GetSize().x;
    SetProgress(m_dragStartProgress + progressDelta);
  }
}

void ProgressBar::OnKeyDown(wxKeyEvent &evt)
{
  double seekDelta = 0;

  switch (evt.GetKeyCode())
  {
  case WXK_LEFT:
    seekDelta = -5.0; // 5 seconds back
    break;
  case WXK_RIGHT:
    seekDelta = 5.0; // 5 seconds forward
    break;
  }

  if (seekDelta != 0 && m_seekCallback)
  {
    double newTime = m_progress * m_duration + seekDelta;
    SetProgress(newTime / m_duration);
    m_seekCallback(newTime);
  }

  evt.Skip();
}

void ProgressBar::OnTimer(wxTimerEvent &evt)
{
  if (m_isLoading)
  {
    UpdateLoadingAnimation();
    Refresh();
  }
}

void ProgressBar::OnSize(wxSizeEvent &evt)
{
  m_surface.reset();
  Refresh();
}

void ProgressBar::SetProgress(double progress)
{
  m_progress = std::clamp(progress, 0.0, 1.0);
  Refresh();
}

void ProgressBar::SetDuration(double duration)
{
  m_duration = std::max(duration, 0.0);
}

void ProgressBar::SetBufferedProgress(double buffered)
{
  m_bufferedProgress = std::clamp(buffered, 0.0, 1.0);
  Refresh();
}

void ProgressBar::AddChapter(const Chapter &chapter)
{
  m_chapters.push_back(chapter);
  Refresh();
}

void ProgressBar::ClearChapters()
{
  m_chapters.clear();
  Refresh();
}

void ProgressBar::SetLoading(bool isLoading)
{
  if (m_isLoading != isLoading)
  {
    m_isLoading = isLoading;
    if (m_isLoading)
    {
      auto FIFTY_MS = 50;
      m_loadingTimer.Start(FIFTY_MS);
    }
    else
    {
      m_loadingTimer.Stop();
    }
    Refresh();
  }
}

void ProgressBar::UpdateLoadingAnimation()
{
  m_loadingAngle += 0.5f;
  if (m_loadingAngle >= 10.0f)
  {
    m_loadingAngle = 0.0f;
  }
}

void ProgressBar::OnSeek(const std::function<void(double)> &callback)
{
  m_seekCallback = callback;
}

double ProgressBar::PixelsToProgress(int x) const
{
  return std::clamp((double)x / GetSize().x, 0.0, 1.0);
}

int ProgressBar::ProgressToPixels(double progress) const
{
  return progress * GetSize().x;
}

Chapter *ProgressBar::GetChapterAtProgress(double progress)
{
  double time = progress * m_duration;
  for (auto &chapter : m_chapters)
  {
    if (time >= chapter.startTime && time < chapter.endTime)
    {
      return &chapter;
    }
  }
  return nullptr;
}