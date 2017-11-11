#include <core/phyr.h>
#include <core/phyr_mem.h>
#include <core/accel/bvh.h>

namespace phyr {

AccelBVH::~AccelBVH() { if (bvhNodes) freeAligned(bvhNodes); }

const int AccelBVH::DEF_MAX_OBJ_PER_NODE = 255;

void AccelBVH::constructBVH() {
    if (objectList.size() == 0) return;

    // Initiate object info list
    size_t sz = objectList.size();
    std::vector<BVHObjectInfo> objectInfoList(sz);

    for (size_t i = 0; i < sz; i++)
        objectInfoList[i] = { i, objectList[i]->worldBounds() };

    // Create a BVH Tree from the object info list
    int nodeCount = 0;
    // Memory pool with a block size of 1MB
    MemoryPool pool(1024 * 1024);
    // Stores the ordered permutation of objectList as defined
    // by the recursive BVH algorithm
    std::vector<std::shared_ptr<Object>> orderedObjectList;

    // Recursively build the BVH Tree
    LOG_INFO("Computing BVH tree...");
    BVHTreeNode* root = constructBVHRecursive(pool, objectInfoList, 0, sz,
                                              &nodeCount, orderedObjectList);
    objectList.swap(orderedObjectList);
    LOG_INFO_FMT("Computed BVH nodes: %d", nodeCount);

    // Compute linear BVH by DFS on {root}
    int linearIdx = 0;
    bvhNodes = allocAligned<LinearBVHNode>(nodeCount);
    // Initilize components in {LinearBVHNode}
    for (int i = 0; i < nodeCount; i++)
        new (&bvhNodes[i]) LinearBVHNode();

    // Transform BVH tree to a BVH linear array
    LOG_INFO("Flattening BVH nodes...");
    flattenBVH(root, &linearIdx);
    ASSERT(linearIdx == nodeCount);
}

constexpr int nBins = 12;
struct BinInfo { int freq; Bounds3f bounds; };

Bounds3f AccelBVH::worldBounds() const {
    return bvhNodes ? bvhNodes[0].bounds : Bounds3f();
}

BVHTreeNode*
AccelBVH::constructBVHRecursive(MemoryPool& pool, std::vector<BVHObjectInfo>& objectInfoList,
                                int startIdx, int end, int* nodeCount,
                                std::vector<std::shared_ptr<Object>>& orderedObjectList) const {
    // Check for trivial invalid case
    if (end - startIdx <= 0) return nullptr;

    (*nodeCount)++;
    BVHTreeNode* node = pool.alloc<BVHTreeNode>();

    // Build bounds of all the objects within this range
    Bounds3f nodeBound = objectInfoList[startIdx].bounds;
    for (int i = startIdx + 1; i < end; i++)
        nodeBound = unionBounds(nodeBound, objectInfoList[i].bounds);

#define CREATE_LEAF_NODE() \
    int offset = orderedObjectList.size(); \
    for (int i = startIdx; i < end; i++) { \
        int idx = objectInfoList[i].objectIdx; \
        orderedObjectList.push_back(objectList[idx]); \
    } \
    node->createLeafNode(offset, range, nodeBound); \
    return node;

    int range = end - startIdx;
    if (range == 1) {
        CREATE_LEAF_NODE();
    } else {
        Bounds3f centroidBounds(objectInfoList[startIdx].centroid);
        for (int i = startIdx + 1; i < end; i++)
            centroidBounds = unionBounds(centroidBounds, objectInfoList[i].centroid);
        int maxDim = centroidBounds.maximumExtent();

        // Check if all centroids lie at the same point
        if (centroidBounds.pMin[maxDim] == centroidBounds.pMax[maxDim]) {
            CREATE_LEAF_NODE();
        } else {
            int mid = (startIdx + end) / 2;

            // @todo: Support other tree split methods
            // Partition primitives using SAH
            if (range <= 2) {
                // Partition into equally sized subsets
                std::nth_element(&objectInfoList[startIdx], &objectInfoList[mid],
                                 &objectInfoList[end-1] + 1,
                                 [maxDim](const BVHObjectInfo& a, const BVHObjectInfo& b) {
                                     return a.centroid[maxDim] < b.centroid[maxDim];
                                 });
            } else {
                BinInfo bins[nBins];
                // Initialize bins
                for (int i = startIdx; i < end; i++) {
                    int bidx = centroidBounds.offset(objectInfoList[i].centroid)[maxDim] * nBins;
                    if (bidx == nBins) bidx--; bins[bidx].freq++;

                    if (bins[bidx].freq == 1)
                        bins[bidx].bounds = objectInfoList[i].bounds;
                    else
                        bins[bidx].bounds = unionBounds(bins[bidx].bounds, objectInfoList[i].bounds);
                }

                // Compute optimum split
                Real cost, minCost = MaxReal, splitIdx = 0;
                Real nodeBoundSurfaceArea = nodeBound.surfaceArea();

                /* @todo Optimize the O(n^2) runtime */
                for (int i = 0; i < nBins - 1; i++) {
                    int f0 = 0, f1 = 0, j;

                    Bounds3f bl = bins[0].bounds;
                    for (j = 1; j <= i; j++) {
                        bl = unionBounds(bl, bins[j].bounds);
                        f0 += bins[j].freq;
                    }

                    Bounds3f br = bins[i + 1].bounds;
                    for (j = i + 2; j < nBins; j++) {
                        br = unionBounds(br, bins[j].bounds);
                        f1 += bins[j].freq;
                    }

                    // Assumed cost of intersection = 1; cost of traversal = 1/8
                    cost = 1 + (f0 * bl.surfaceArea() + f1 * br.surfaceArea()) / nodeBoundSurfaceArea;
                    if (cost < minCost) { minCost = cost; splitIdx = i; }
                }

                // Since cost of intersection is 1, cost of a leaf is {range}
                Real leafCost = range;
                if (range > maxObjectsPerNode || minCost < leafCost) {
                    // Split objects around optimum split
                    BVHObjectInfo* nmid = std::partition(
                                &objectInfoList[startIdx], &objectInfoList[end-1] + 1,
                                [=](const BVHObjectInfo& info) {
                                    int bidx = centroidBounds.offset(info.centroid)[maxDim] * nBins;
                                    if (bidx == nBins) bidx--;
                                    return bidx <= splitIdx;
                                });
                    mid = nmid - &objectInfoList[0];
                } else {
                    CREATE_LEAF_NODE();
                }
            }

            BVHTreeNode* lc = constructBVHRecursive(pool, objectInfoList, startIdx, mid,
                                                    nodeCount, orderedObjectList);
            BVHTreeNode* rc = constructBVHRecursive(pool, objectInfoList, mid, end,
                                                    nodeCount, orderedObjectList);
            node->createInteriorNode(maxDim, lc, rc);
        }
    }

#undef CREATE_LEAF_NODE

    return node;
}

int AccelBVH::flattenBVH(BVHTreeNode* treeNode, int* linearIdx) {
    // Copy data from tree node
    LinearBVHNode* linearNode = &bvhNodes[*linearIdx];

    linearNode->bounds = treeNode->bounds;
    int currentIdx = (*linearIdx)++;

    // If tree node is a leaf
    if (treeNode->nObjects > 0) {
        linearNode->objectStartIdx = treeNode->startIdx;
        linearNode->nObjects = treeNode->nObjects;
    } else {
        linearNode->nObjects = 0;
        linearNode->splitAxis = treeNode->splitAxis;
        // Recursive traverse left child
        flattenBVH(treeNode->child[0], linearIdx);
        // Store index of right child with a recursive call
        linearNode->secondChildIdx = flattenBVH(treeNode->child[1], linearIdx);
    }

    return currentIdx;
}

bool AccelBVH::intersectRay(const Ray& ray, SurfaceInteraction* si) const {
    bool intersected = false;
    // Initialize ray intersection parameters
    Vector3f invDir(1 / ray.d.x, 1 / ray.d.y, 1 / ray.d.z);
    int isDirNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };

    // Test against each BVH node
    int nodeStack[64];
    int stackSize = 0, itrIdx = 0, i;

    while (true) {
        const LinearBVHNode* node = &bvhNodes[itrIdx];
        // Inspect current node
        if (node->bounds.intersectRay(ray, invDir, isDirNeg)) {
            if (node->nObjects > 0) {
                // Node is leaf. Test against all objects in leaf
                for (i = 0; i < node->nObjects; i++)
                    if (objectList[node->objectStartIdx + i]->intersectRay(ray, si))
                        intersected = true;
                if (stackSize == 0) break;
                itrIdx = nodeStack[--stackSize];
            } else {
                // Internal node. Test near nodes first. Put far nodes in the stack
                if (isDirNeg[node->splitAxis]) {
                    // Stash first child and inspect second child first
                    nodeStack[stackSize++] = itrIdx + 1;
                    itrIdx = node->secondChildIdx;
                } else {
                    // Stash second child and inspect first child first
                    nodeStack[stackSize++] = node->secondChildIdx;
                    itrIdx++;
                }
            }
        } else {
            if (stackSize == 0) break;
            itrIdx = nodeStack[--stackSize];
        }
    }

    return intersected;
}

bool AccelBVH::intersectRay(const Ray& ray) const {
    // Initialize ray intersection parameters
    Vector3f invDir(1 / ray.d.x, 1 / ray.d.y, 1 / ray.d.z);
    int isDirNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };

    // Test against each BVH node
    int nodeStack[64];
    int stackSize = 0, itrIdx = 0, i;

    while (true) {
        const LinearBVHNode* node = &bvhNodes[itrIdx];
        // Inspect current node
        if (node->bounds.intersectRay(ray, invDir, isDirNeg)) {
            if (node->nObjects > 0) {
                // Node is leaf. Test against all objects in leaf
                for (i = 0; i < node->nObjects; i++)
                    if (objectList[node->objectStartIdx + i]->intersectRay(ray))
                        return true;
                if (stackSize == 0) break;
                itrIdx = nodeStack[--stackSize];
            } else {
                // Internal node. Test near nodes first. Put far nodes in the stack
                if (isDirNeg[node->splitAxis]) {
                    // Stash first child and inspect second child first
                    nodeStack[stackSize++] = itrIdx + 1;
                    itrIdx = node->secondChildIdx;
                } else {
                    // Stash second child and inspect first child first
                    nodeStack[stackSize++] = node->secondChildIdx;
                    itrIdx++;
                }
            }
        } else {
            if (stackSize == 0) break;
            itrIdx = nodeStack[--stackSize];
        }
    }

    return false;
}

std::shared_ptr<AccelBVH> createBVHAccel(const std::vector<std::shared_ptr<Object>>& objList,
                                         int maxObjectsPerNode, const TreeSplitMethod tsp) {
    return std::make_shared<AccelBVH>(objList, maxObjectsPerNode, tsp);
}

}  // namespace phyr
