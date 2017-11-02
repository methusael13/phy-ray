#ifndef PHYRAY_CORE_IMAGEIO_H
#define PHYRAY_CORE_IMAGEIO_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>

namespace phyr {

// Declaration of all image formats supported by PhyRay
enum class ImageFormat { EXR };

class ImageIO {
  public:
    static
    void writeImage(const std::string& filename, const Real* rgb,
                    const Bounds2i& outputBounds, const Point2i& resolution,
                    ImageFormat format = ImageFormat::EXR);

  private:
    // Disable class construction
    ImageIO() {}
};

}  // namespace phyr

#endif
