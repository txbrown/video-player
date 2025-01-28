#include <gtest/gtest.h>
#include "progress_bar.h"
#include <wx/wx.h>

class ProgressBarTest : public testing::Test
{
protected:
    void SetUp() override
    {

        frame = new wxFrame(nullptr, wxID_ANY, "Test Frame");
        progressBar = new ProgressBar(frame);
    }

    void TearDown() override
    {
        frame->Destroy();
        wxEntryCleanup();
    }

    wxFrame *frame;
    ProgressBar *progressBar;
};

TEST_F(ProgressBarTest, InitialState)
{
    EXPECT_DOUBLE_EQ(0.0, progressBar->GetProgress());
    EXPECT_FALSE(progressBar->IsLoading());
}

TEST_F(ProgressBarTest, SetProgress)
{
    progressBar->SetProgress(0.5);
    EXPECT_DOUBLE_EQ(0.5, progressBar->GetProgress());

    progressBar->SetProgress(-0.1);
    EXPECT_DOUBLE_EQ(0.0, progressBar->GetProgress());

    progressBar->SetProgress(1.5);
    EXPECT_DOUBLE_EQ(1.0, progressBar->GetProgress());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}