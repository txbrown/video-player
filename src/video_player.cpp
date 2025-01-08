#include "video_player.h"

VideoPlayer::VideoPlayer()
    : wxFrame(nullptr, wxID_ANY, "Video Player Demo"), m_isPlaying(false), m_currentTime(0.0)
{
    SetBackgroundColour(*wxBLACK);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *controlsSizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->AddStretchSpacer();

    m_progressBar = new ProgressBar(this);
    m_skipBackButton = new SkipButton(this, BACKWARD);
    m_skipForwardButton = new SkipButton(this, FORWARD);
    m_playButton = new PlayButton(this);
    m_pauseButton = new PauseButton(this);

    controlsSizer->Add(m_skipBackButton, 0, wxEXPAND | wxALL, 10);
    controlsSizer->Add(m_playButton, 0, wxEXPAND | wxALL, 10);
    controlsSizer->Add(m_pauseButton, 0, wxEXPAND | wxALL, 10);
    controlsSizer->Add(m_skipForwardButton, 0, wxEXPAND | wxALL, 10);
    m_pauseButton->Hide();

    sizer->Add(m_progressBar, 0, wxEXPAND | wxALL, 10);
    sizer->Add(controlsSizer, 0, wxEXPAND | wxALL, 10);

    SetSizer(sizer);

    m_playButton->OnPress([this]()
                          { TogglePlay(); });
    m_pauseButton->OnPress([this]()
                           { TogglePlay(); });
    m_progressBar->OnSeek([this](double time)
                          {
        m_currentTime = time;
        UpdateProgressBar();
        SimulateBuffering(); });

    auto TOTAL_DURATION = 60.0;
    m_video = std::make_unique<Video>();
    m_video->source = "path/to/video.mp4";
    m_video->title = "Demo Video";
    m_video->duration = TOTAL_DURATION;
    m_video->chapters = {
        {"Introduction", 0.0, 10.0},
        {"Chapter 1: Getting Started", 20.0, 3.0},
        {"Chapter 2: Main Content", 40.0, 40.0},
        {"Conclusion", 420.0, 50}};

    m_progressBar->SetDuration(m_video->duration);

    for (const auto &chapter : m_video->chapters)
    {
        m_progressBar->AddChapter(chapter);
    }

    m_progressBar->SetBufferedProgress(0.3);

    m_progressBar->OnSeek([this](double time)
                          {
        m_currentTime = time;
        UpdateProgressBar();
        SimulateBuffering(); });

    m_playbackTimer.Bind(wxEVT_TIMER, &VideoPlayer::OnTimer, this);
    auto ONE_HUNDRED_MS = 100;
    m_playbackTimer.Start(ONE_HUNDRED_MS);
    m_bufferingTimer.Bind(wxEVT_TIMER, &VideoPlayer::OnBufferingTimer, this);
}

void VideoPlayer::OnTimer(wxTimerEvent &evt)
{
    if (!m_isPlaying)
    {
        return;
    }
    auto ONE_HUNDRED_MS = 0.1;
    m_currentTime += ONE_HUNDRED_MS;
    if (m_currentTime >= m_video->duration)
    {
        m_currentTime = 0.0;
        m_isPlaying = false;
    }
    UpdateProgressBar();
}

void VideoPlayer::OnBufferingTimer(wxTimerEvent &evt)
{
    m_bufferingTimer.Stop();
    m_progressBar->SetLoading(false);
    m_bufferedProgress = std::min(m_bufferedProgress + 0.1, 1.0);
    m_progressBar->SetBufferedProgress(m_bufferedProgress);
}

void VideoPlayer::UpdateProgressBar()
{
    m_progressBar->SetProgress(this->progress());
}

void VideoPlayer::SimulateBuffering()
{
    m_progressBar->SetLoading(true);
    m_isPlaying = false;
    auto TWO_SECONDS = 2000;
    m_bufferingTimer.Start(TWO_SECONDS, true);
}

void VideoPlayer::Play()
{
    m_isPlaying = true;
    m_playButton->Hide();
    m_pauseButton->Show();
    GetSizer()->Layout();
}

void VideoPlayer::Pause()
{
    m_isPlaying = false;
    m_pauseButton->Hide();
    m_playButton->Show();
    GetSizer()->Layout();
}

void VideoPlayer::TogglePlay()
{
    m_isPlaying = !m_isPlaying;
    if (m_isPlaying)
    {
        Play();
    }
    else
    {
        Pause();
    }
}

const double VideoPlayer::progress() const
{
    return m_currentTime / m_video->duration;
}