#ifndef PHYRAY_ACCEL_BVH_H
#define PHYRAY_ACCEL_BVH_H

#include <core/phyr.h>
#include <core/geometry/geometry.h>
#include <core/object/object.h>

#include <memory>

namespace phyr {

typedef struct _BVHTreeNode BVHTreeNode;
typedef struct _BVHObjectInfo BVHObjectInfo;

struct _BVHObjectInfo {
    _BVHObjectInfo(size_t objectIdx, const Bounds3f& bounds) :
        objectIdx(objectIdx), bounds(bounds),
        centroid(0.5 * bounds.pMin + 0.5 * bounds.pMax) {}

    size_t objectIdx; Bounds3f bounds;
    Point3f centroid;
};

struct _BVHTreeNode {
    void createLeafNode(int sIdx, int n, const Bounds3f& b) {
        startIdx = sIdx; nObjects = n; bounds = b;
        child[0] = child[1] = nullptr;
    }

    void createInteriorNode(int spAxis, BVHTreeNode* lc, BVHTreeNode* rc) {
        child[0] = lc; child[1] = rc;
        splitAxis = spAxis; nObjects = 0;
        bounds = unionBounds(lc->bounds, rc->bounds);
    }

    Bounds3f bounds;
    BVHTreeNode* child[2];

    // Since objects in a node are to be ordered in a contiguous manner,
    // we represent them as an ordered set of object indexes
    // [{startIdx}, {startIdx + nObjects}] instead of a list of arbitrarily
    // ordered object indexes. The indexes refer to the elements in
    // {AccelBVH}'s {objectList} member
    int splitAxis, startIdx, nObjects;
};

class AccelBVH {
  public:
    // Support Surface Area Heuristic for tree splitting
    enum class TreeSplitMethod { SAH };
    static const int DEF_MAX_OBJ_PER_NODE = 255;

    AccelBVH(const std::vector<std::shared_ptr<Object>>& objList,
             const int maxObjectsPerNode, const TreeSplitMethod tspMethod) :
        // Ensure {maxObjectsPerNode} does not exceed {DEF_MAX_OBJ_PER_NODE}
        maxObjectsPerNode(std::min(DEF_MAX_OBJ_PER_NODE, maxObjectsPerNode)),
        tspMethod(tspMethod), objectList(objList) { constructBVH(); }

  private:
    void constructBVH();
    /**
     * Recursively builds the BVH tree with object range [{startIdx}, {end}).
     * @returns The root of the built tree as a pointer to {BVHTreeNode}
     */
    BVHTreeNode* constructBVHRecursive(MemoryPool& pool, std::vector<BVHObjectInfo>& objectInfoList,
                                       int startIdx, int end, int* nodeCount,
                                       std::vector<std::shared_ptr<Object>>& orderedObjectList) const;

    const int maxObjectsPerNode;
    const TreeSplitMethod tspMethod;
    std::vector<std::shared_ptr<Object>> objectList;
};

}  // namespace phyr

#endif
