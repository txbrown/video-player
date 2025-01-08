#pragma once
#include <wx/bitmap.h>
#include "include/core/SkImage.h"
#include "include/core/SkSurface.h"

namespace utils
{

    /**
     * Converts a Skia surface to a wxBitmap
     * @param surface The Skia surface to convert
     * @return wxBitmap The resulting wxBitmap, or an invalid bitmap if conversion fails
     */
    wxBitmap SkiaSurfaceToWxBitmap(sk_sp<SkSurface> &surface);

} // namespace utils