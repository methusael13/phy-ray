#ifndef PHYRAY_CORE_SHAPE_H
#define PHYRAY_CORE_SHAPE_H

#include <core/phyr.h>

namespace phyr {

class Shape {
  public:
    bool reverseNormals, transformChangesCoordSys;
};

}

#endif
