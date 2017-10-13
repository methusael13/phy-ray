#ifndef PHYRAY_ACCEL_BVH_H
#define PHYRAY_ACCEL_BVH_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>
#include <core/object/object.h>

#include <memory>

namespace phyr {

typedef struct _BHVObjectInfo BHVObjectInfo;

struct _BHVObjectInfo {
    _BHVObjectInfo(size_t objectIdx, const Bounds3f& bounds) :
        objectIdx(objectIdx), bounds(bounds),
        centroid(0.5 * bounds.pMin + 0.5 * bounds.pMax) {}

    size_t objectIdx;
    Bounds3f bounds;
    Point3f centroid;
};

class AccelBVH {
  public:
    // Support Surface Area Heuristic for tree splitting
    enum class TreeSplitMethod { SAH };
    static const int DEF_MAX_OBJ_PER_NODE = 255;

    AccelBVH(const std::vector<std::shared_ptr<Object>>& objList,
             const int maxObjectsPerNode, const TreeSplitMethod tspMethod) :
        maxObjectsPerNode(std::min(DEF_MAX_OBJ_PER_NODE, maxObjectsPerNode)),
        tspMethod(tspMethod), objectList(objList) {
        if (objectList.size() == 0) return;

        // Initialize object info list for given objects
        std::vector<BHVObjectInfo> objectInfoList(objectList.size());
        for (size_t i = 0; i < objectList.size(); i++)
            objectInfoList[i] = { i, objectList[i]->worldBounds() };

        // Build BVH Tree using object info list
    }

  private:
    const int maxObjectsPerNode;
    const TreeSplitMethod tspMethod;
    std::vector<std::shared_ptr<Object>> objectList;
};

}  // namespace phyr

#endif
