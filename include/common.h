#pragma once

#include <include/core/SkColor.h>
#define COLOR_WHITE SK_ColorWHITE
#define COLOR_BLACK SK_ColorBLACK
#define COLOR_RED SK_ColorRED
#define COLOR_GREEN SK_ColorGREEN
#define COLOR_BLUE SK_ColorBLUE

namespace Colors
{
    const SkColor DARK_GRAY = SkColorSetRGB(200, 200, 200);
    const SkColor LIGHT_GRAY = SkColorSetRGB(230, 230, 230);
    const SkColor WHITE = SK_ColorWHITE;
    const SkColor BLACK = SK_ColorBLACK;
    const SkColor YOUTUBE_RED = SkColorSetRGB(255, 0, 0);
}

enum DefaultButtonStyle
{
    DEFAULT_STYLE = 0,
    ROUNDED = 1,
    WITH_LABEL = 2,
    HOVER_EFFECT = 4
};