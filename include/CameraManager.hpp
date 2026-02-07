#pragma once
#include "raylib.h"
#include <memory>

namespace TimeMaster {

/**
 * @brief Manages camera positioning and updates for 3D view
 * Supports both static and third-person following modes
 */
class CameraManager {
private:
    Camera3D m_camera;
    
    // Third-person camera settings
    float m_distance;           // Distance from player
    float m_height;             // Height above player
    float m_angleAroundPlayer;  // Horizontal rotation angle
    float m_pitch;              // Vertical angle
    bool m_isThirdPerson;       // Toggle third-person mode
    
    // Mouse control
    float m_mouseSensitivity;
    bool m_cursorLocked;
    
public:
    CameraManager();
    
    /**
     * @brief Get the raylib Camera3D structure
     */
    Camera3D& GetCamera() { return m_camera; }
    const Camera3D& GetCamera() const { return m_camera; }
    
    /**
     * @brief Update camera to follow player (third-person)
     */
    void UpdateThirdPerson(Vector3 playerPosition, float deltaTime);
    
    /**
     * @brief Update camera with mouse input (always active)
     */
    void HandleMouseInput(float deltaTime);
    
    /**
     * @brief Get camera forward direction (for movement)
     */
    Vector3 GetForwardDirection() const;
    
    /**
     * @brief Get camera right direction (for movement)
     */
    Vector3 GetRightDirection() const;
    
    /**
     * @brief Toggle between static and third-person mode
     */
    void ToggleMode();
    
    /**
     * @brief Reset camera to default position
     */
    void Reset();
    
    /**
     * @brief Check if in third-person mode
     */
    bool IsThirdPerson() const { return m_isThirdPerson; }
    
    /**
     * @brief Adjust camera distance (zoom)
     */
    void AdjustDistance(float delta);
    
    /**
     * @brief Toggle cursor lock (for menu access)
     */
    void ToggleCursorLock();
    
    /**
     * @brief Check if cursor is locked
     */
    bool IsCursorLocked() const;
};

} // namespace TimeMaster
