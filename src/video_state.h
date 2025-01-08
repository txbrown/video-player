#pragma once
#include <string>
#include <vector>

struct Chapter
{
  std::string title;
  double startTime;
  double endTime;
};

struct Video
{
  std::string source;
  std::string title;
  double duration;
  std::vector<Chapter> chapters;
};