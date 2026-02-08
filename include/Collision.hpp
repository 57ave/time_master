#pragma once
#include "raylib.h"
#include "raymath.h"

namespace TimeMaster {

/**
 * @brief Axis-Aligned Bounding Box for rectangular collision
 */
struct AABB {
    Vector3 min;  // Bottom-left-back corner
    Vector3 max;  // Top-right-front corner
    
    /**
     * @brief Create AABB from center position and half-extents
     */
    static AABB FromCenter(Vector3 center, Vector3 halfExtents) {
        return {
            Vector3Subtract(center, halfExtents),
            Vector3Add(center, halfExtents)
        };
    }
    
    /**
     * @brief Get center position of AABB
     */
    Vector3 GetCenter() const {
        return Vector3Scale(Vector3Add(min, max), 0.5f);
    }
    
    /**
     * @brief Get half-extents (size/2) of AABB
     */
    Vector3 GetHalfExtents() const {
        return Vector3Scale(Vector3Subtract(max, min), 0.5f);
    }
    
    /**
     * @brief Check if this AABB intersects with another
     */
    bool Intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
    
    /**
     * @brief Draw debug visualization of AABB
     */
    void DrawDebug(Color color) const {
        Vector3 size = Vector3Subtract(max, min);
        Vector3 center = GetCenter();
        DrawCubeWires(center, size.x, size.y, size.z, color);
    }
};

/**
 * @brief Collision resolution data
 */
struct CollisionResolution {
    bool hasCollision;
    Vector3 pushback;  // Vector to push entities apart
};

/**
 * @brief Resolve collision between two AABBs in 3D (horizontal planes only)
 * Returns pushback vector to separate them on X and Z axes
 * Ignores Y axis for ground-based 3D movement
 */
inline CollisionResolution ResolveAABBCollision(const AABB& a, const AABB& b) {
    if (!a.Intersects(b)) {
        return {false, {0, 0, 0}};
    }
    
    // Get centers for direction calculation
    Vector3 centerA = a.GetCenter();
    Vector3 centerB = b.GetCenter();
    
    // Calculate overlap on horizontal axes (X and Z only)
    float overlapX = (a.max.x < b.max.x) ? 
        (a.max.x - b.min.x) : (b.max.x - a.min.x);
    float overlapZ = (a.max.z < b.max.z) ? 
        (a.max.z - b.min.z) : (b.max.z - a.min.z);
    
    // Find minimum overlap axis (axis of least penetration) on horizontal plane
    Vector3 pushback = {0, 0, 0};
    
    if (overlapX < overlapZ) {
        // Push along X axis (left/right)
        pushback.x = (centerA.x < centerB.x) ? -overlapX : overlapX;
    } else {
        // Push along Z axis (forward/backward)
        pushback.z = (centerA.z < centerB.z) ? -overlapZ : overlapZ;
    }
    
    // Y axis is not affected (entities stay at their height)
    
    return {true, pushback};
}

/**
 * @brief Resolve collision between two AABBs with full 3D pushback
 * This version considers all three axes including Y
 */
inline CollisionResolution ResolveAABBCollision3D(const AABB& a, const AABB& b) {
    if (!a.Intersects(b)) {
        return {false, {0, 0, 0}};
    }
    
    // Get centers for direction calculation
    Vector3 centerA = a.GetCenter();
    Vector3 centerB = b.GetCenter();
    
    // Calculate overlap on each axis
    float overlapX = (a.max.x < b.max.x) ? 
        (a.max.x - b.min.x) : (b.max.x - a.min.x);
    float overlapY = (a.max.y < b.max.y) ? 
        (a.max.y - b.min.y) : (b.max.y - a.min.y);
    float overlapZ = (a.max.z < b.max.z) ? 
        (a.max.z - b.min.z) : (b.max.z - a.min.z);
    
    // Find minimum overlap axis (axis of least penetration)
    Vector3 pushback = {0, 0, 0};
    
    if (overlapX < overlapY && overlapX < overlapZ) {
        // Push along X axis
        pushback.x = (centerA.x < centerB.x) ? -overlapX : overlapX;
    } else if (overlapY < overlapZ) {
        // Push along Y axis
        pushback.y = (centerA.y < centerB.y) ? -overlapY : overlapY;
    } else {
        // Push along Z axis
        pushback.z = (centerA.z < centerB.z) ? -overlapZ : overlapZ;
    }
    
    return {true, pushback};
}

/**
 * @brief Check collision between AABB and sphere
 * Used for projectiles and collectibles collision with rectangular entities
 */
inline bool CheckAABBSphereCollision(const AABB& box, Vector3 sphereCenter, float sphereRadius) {
    // Find the closest point on the AABB to the sphere center
    Vector3 closestPoint;
    closestPoint.x = Clamp(sphereCenter.x, box.min.x, box.max.x);
    closestPoint.y = Clamp(sphereCenter.y, box.min.y, box.max.y);
    closestPoint.z = Clamp(sphereCenter.z, box.min.z, box.max.z);
    
    // Calculate distance between sphere center and closest point
    float distanceSquared = Vector3LengthSqr(Vector3Subtract(sphereCenter, closestPoint));
    
    // Check if distance is less than radius
    return distanceSquared <= (sphereRadius * sphereRadius);
}

/**
 * @brief Constrain an AABB within arena boundaries
 * @param aabb The bounding box to constrain
 * @param arenaSize Half-size of the square arena
 * @return The position adjustment needed to keep AABB inside arena
 */
inline Vector3 ConstrainAABBToArena(const AABB& aabb, float arenaSize) {
    Vector3 adjustment = {0, 0, 0};
    
    // Check and constrain X axis
    if (aabb.min.x < -arenaSize) {
        adjustment.x = -arenaSize - aabb.min.x;
    } else if (aabb.max.x > arenaSize) {
        adjustment.x = arenaSize - aabb.max.x;
    }
    
    // Check and constrain Z axis
    if (aabb.min.z < -arenaSize) {
        adjustment.z = -arenaSize - aabb.min.z;
    } else if (aabb.max.z > arenaSize) {
        adjustment.z = arenaSize - aabb.max.z;
    }
    
    // Y axis typically not constrained for ground-based movement
    
    return adjustment;
}

} // namespace TimeMaster
