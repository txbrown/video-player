#pragma once

#include <wx/wx.h>
#include "progress_bar.h"
#include "video_state.h"
#include "skip_button.h"
#include "play_button.h"
#include "pause_button.h"

class VideoPlayer : public wxFrame
{
public:
    VideoPlayer();
    virtual ~VideoPlayer() = default;

    void Play();
    void Pause();
    void Stop();
    void Seek(double seconds);
    void SkipBackward();
    void SkipForward();

private:
    // UI Components
    ProgressBar *m_progressBar;
    SkipButton *m_skipBackButton;
    SkipButton *m_skipForwardButton;
    PlayButton *m_playButton;
    PauseButton *m_pauseButton;
    std::unique_ptr<Video> m_video;

    // Timers
    wxTimer m_playbackTimer;
    wxTimer m_bufferingTimer;

    // State
    bool m_isPlaying{false};
    double m_currentTime;
    double m_bufferedProgress{0.3};
    const double progress() const;

    // Event handlers
    void OnTimer(wxTimerEvent &evt);
    void OnBufferingTimer(wxTimerEvent &evt);

    void UpdateProgressBar();
    void SimulateBuffering();
    void TogglePlay();
};