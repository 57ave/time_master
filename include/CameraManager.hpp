#pragma once
#include "raylib.h"
#include <memory>

namespace TimeMaster {

/**
 * @brief Manages camera positioning and updates for 3D view
 */
class CameraManager {
private:
    Camera3D m_camera;
    
public:
    CameraManager();
    
    /**
     * @brief Get the raylib Camera3D structure
     */
    Camera3D& GetCamera() { return m_camera; }
    const Camera3D& GetCamera() const { return m_camera; }
    
    /**
     * @brief Update camera based on player position (optional)
     */
    void Update(Vector3 targetPosition);
    
    /**
     * @brief Reset camera to default position
     */
    void Reset();
};

} // namespace TimeMaster
