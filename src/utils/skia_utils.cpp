#include "skia_utils.h"

namespace utils
{

    wxBitmap SkiaSurfaceToWxBitmap(sk_sp<SkSurface> &surface)
    {
        sk_sp<SkImage> image = surface->makeImageSnapshot();
        if (!image)
        {
            return wxBitmap();
        }

        SkPixmap pixmap;
        if (!image->peekPixels(&pixmap))
        {
            return wxBitmap();
        }

        wxImage wx_image(
            pixmap.width(),
            pixmap.height(),
            static_cast<unsigned char *>(malloc(pixmap.width() * pixmap.height() * 3)));

        const unsigned char *src = static_cast<const unsigned char *>(pixmap.addr());
        unsigned char *dst = wx_image.GetData();

        for (int y = 0; y < pixmap.height(); y++)
        {
            for (int x = 0; x < pixmap.width(); x++)
            {
                int srcIdx = y * pixmap.rowBytes() + x * 4;
                int dstIdx = (y * pixmap.width() + x) * 3;
                dst[dstIdx + 0] = src[srcIdx + 0];
                dst[dstIdx + 1] = src[srcIdx + 1];
                dst[dstIdx + 2] = src[srcIdx + 2];
            }
        }

        return wxBitmap(wx_image);
    }

} // namespace utils