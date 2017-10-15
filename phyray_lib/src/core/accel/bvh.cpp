#include <core/phyr.h>
#include <core/phyr_mem.h>
#include <core/accel/bvh.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"

namespace phyr {

void AccelBVH::constructBVH() {
    if (objectList.size() == 0) return;

    // Initiate object info list
    size_t sz = objectList.size();
    std::vector<BVHObjectInfo> objectInfoList(sz);
    for (size_t i = 0; i < sz; i++) {
        objectInfoList[i] = { i, objectList[i]->worldBounds() };
    }

    // Create a BVH Tree from the object info list
    int nodeCount = 0;
    // Memory pool with a block size of 1MB
    MemoryPool pool(1024 * 1024);
    // Stores the ordered permutation of objectList as defined by the recursive BVH algorithm
    std::vector<std::shared_ptr<Object>> orderedObjectList;

    // Recursively build the BVH Tree
    BVHTreeNode* root = constructBVHRecursive(pool, objectInfoList, 0, sz,
                                          &nodeCount, orderedObjectList);
    objectList.swap(orderedObjectList);
}

BVHTreeNode*
AccelBVH::constructBVHRecursive(MemoryPool& pool, std::vector<BVHObjectInfo>& objectInfoList,
                            int start, int end, int* nodeCount,
                            std::vector<std::shared_ptr<Object>>& orderedObjectList) {

    return nullptr;
}

}  // namespace phyr

#pragma GCC diagnostic pop
