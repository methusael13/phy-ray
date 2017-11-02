#include <core/debug.h>
#include <core/imageio.h>

#include <memory>
#include <ImfRgba.h>
#include <ImfRgbaFile.h>

namespace phyr {

inline bool hasExtension(const std::string& filename, const std::string& ext) {
    if (ext.length() >= filename.length()) return false;
    return std::equal(ext.rbegin(), ext.rend(), filename.rbegin(),
                      [](char a, char b) { return std::tolower(a) == std::tolower(b); });
}

inline std::string sanitizeFilename(const std::string& filename, const std::string& ext) {
    if (hasExtension(filename, ext))
        return filename;
    return filename + ext;
}

// Image IO utilities for various formats
void writeImageEXR(const std::string& imgFilename, const Real* rgb, int xRes, int yRes,
                   int xTotalRes, int yTotalRes, int xOffset, int yOffset) {
    size_t nPixels = xRes * yRes;
    // Construct internal Rgba struct from input rgb array
    std::unique_ptr<Imf::Rgba[]> pixels(new Imf::Rgba[nPixels]);

    for (size_t i = 0; i < nPixels; i++)
        pixels[i] = { rgb[3 * i], rgb[3 * i + 1], rgb[3 * i + 2] };

    // Create OpenEXR bounds
    Imath::Box2i displayWindow(Imath::V2i(0, 0), Imath::V2i(xTotalRes - 1, yTotalRes - 1));
    Imath::Box2i dataWindow(Imath::V2i(xOffset, yOffset),
                            Imath::V2i(xOffset + xRes - 1, yOffset + yRes - 1));

    try {
        // Write image data to file
        Imf::RgbaOutputFile file(imgFilename.c_str(), displayWindow, dataWindow, Imf::WRITE_RGBA);
        file.setFrameBuffer(pixels.get() - xOffset - yOffset * xRes, 1, xRes);
        file.writePixels(yRes);
    } catch (const std::exception& ex) {
        LOG_ERR_FMT("Unable to write image to file: %s", imgFilename.c_str());
    }
}

void ImageIO::writeImage(const std::string& filename, const Real* rgb, const Bounds2i& outputBounds,
                         const Point2i& resolution, ImageFormat format) {
    // Cropped image resolution
    Vector2i croppedResolution = outputBounds.diagonal();

    // Write image data as specified by format
    switch (format) {
        case ImageFormat::EXR: {
            // Sanitize filename
            std::string imageFile = sanitizeFilename(filename, ".exr");
            writeImageEXR(imageFile, rgb, croppedResolution.x, croppedResolution.y,
                          resolution.x, resolution.y, outputBounds.pMin.x, outputBounds.pMin.y);
        } break;
    }
}

}  // namespace phyr
