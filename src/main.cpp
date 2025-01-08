#include "video_player.h"
#include <wx/wx.h>

class VideoPlayerApp : public wxApp
{
public:
  bool OnInit() override
  {
    if (!wxApp::OnInit())
    {
      return false;
    }

    VideoPlayer *videoPlayerFrame = new VideoPlayer();
    videoPlayerFrame->SetClientSize(800, 600);
    videoPlayerFrame->Center();
    videoPlayerFrame->Show();

    return true;
  }
};

wxIMPLEMENT_APP(VideoPlayerApp);
