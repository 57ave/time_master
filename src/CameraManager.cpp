#include "CameraManager.hpp"
#include "Config.hpp"
#include "raymath.h"
#include <cmath>

namespace TimeMaster {

CameraManager::CameraManager() 
    : m_distance(CAMERA_DISTANCE)
    , m_height(CAMERA_HEIGHT)
    , m_angleAroundPlayer(0.0f)
    , m_pitch(20.0f)
    , m_isThirdPerson(true)  // Start in third-person by default
    , m_mouseSensitivity(0.15f)
    , m_cursorLocked(true) {
    
    // Disable cursor and lock it to the window for FPS-style control
    DisableCursor();
    
    Reset();
}

void CameraManager::Reset() {
    if (m_isThirdPerson) {
        // Third-person: camera behind and above player
        m_camera.position = {0.0f, m_height, m_distance};
        m_camera.target = {0.0f, 50.0f, 0.0f};
    } else {
        // Static/overview mode
        m_camera.position = {-CAMERA_DISTANCE, CAMERA_HEIGHT, CAMERA_DISTANCE};
        m_camera.target = {0.0f, 50.0f, 0.0f};
    }
    
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fovy = CAMERA_FOV;
    m_camera.projection = CAMERA_PERSPECTIVE;
    
    m_angleAroundPlayer = 0.0f;
    m_pitch = 20.0f;
}

void CameraManager::UpdateThirdPerson(Vector3 playerPosition, float deltaTime) {
    if (!m_isThirdPerson) {
        // Static camera mode - don't follow
        return;
    }
    
    // Handle mouse input for camera rotation (always active, no button needed)
    HandleMouseInput(deltaTime);
    
    // Handle mouse wheel for zoom
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        AdjustDistance(-wheel * 20.0f);
    }
    
    // Calculate camera position based on angle and distance
    float horizontalDistance = m_distance * cosf(m_pitch * DEG2RAD);
    float verticalDistance = m_distance * sinf(m_pitch * DEG2RAD);
    
    // Calculate offset from player
    float offsetX = horizontalDistance * sinf(m_angleAroundPlayer * DEG2RAD);
    float offsetZ = horizontalDistance * cosf(m_angleAroundPlayer * DEG2RAD);
    
    // Set camera position relative to player
    m_camera.position.x = playerPosition.x - offsetX;
    m_camera.position.y = playerPosition.y + verticalDistance;
    m_camera.position.z = playerPosition.z - offsetZ;
    
    // Camera looks at player (slightly above to see better)
    m_camera.target = playerPosition;
    m_camera.target.y += 30.0f;
}

void CameraManager::HandleMouseInput(float deltaTime) {
    (void)deltaTime; // Not used for now
    
    // Get mouse movement delta (works with disabled cursor)
    Vector2 mouseDelta = GetMouseDelta();
    
    // Update angles based on mouse movement
    m_angleAroundPlayer -= mouseDelta.x * m_mouseSensitivity;
    m_pitch += mouseDelta.y * m_mouseSensitivity;  // Inverted: + instead of -
    
    // Clamp pitch to avoid flipping
    if (m_pitch < 5.0f) m_pitch = 5.0f;
    if (m_pitch > 80.0f) m_pitch = 80.0f;
    
    // Wrap angle around player (optional, but keeps values clean)
    while (m_angleAroundPlayer > 360.0f) m_angleAroundPlayer -= 360.0f;
    while (m_angleAroundPlayer < 0.0f) m_angleAroundPlayer += 360.0f;
}

Vector3 CameraManager::GetForwardDirection() const {
    // Get horizontal forward direction (ignore pitch for movement)
    Vector3 forward;
    forward.x = sinf(m_angleAroundPlayer * DEG2RAD);
    forward.y = 0.0f; // Keep movement horizontal
    forward.z = cosf(m_angleAroundPlayer * DEG2RAD);
    return Vector3Normalize(forward);
}

Vector3 CameraManager::GetRightDirection() const {
    // Right is perpendicular to forward on horizontal plane
    Vector3 forward = GetForwardDirection();
    Vector3 up = {0.0f, 1.0f, 0.0f};
    return Vector3Normalize(Vector3CrossProduct(forward, up));
}

void CameraManager::ToggleMode() {
    m_isThirdPerson = !m_isThirdPerson;
    Reset();
}

void CameraManager::AdjustDistance(float delta) {
    m_distance += delta;
    
    // Clamp distance
    if (m_distance < 100.0f) m_distance = 100.0f;
    if (m_distance > 800.0f) m_distance = 800.0f;
}

void CameraManager::ToggleCursorLock() {
    if (m_cursorLocked) {
        // Unlock cursor
        EnableCursor();
        m_cursorLocked = false;
    } else {
        // Lock cursor
        DisableCursor();
        m_cursorLocked = true;
    }
}

bool CameraManager::IsCursorLocked() const {
    return m_cursorLocked;
}

} // namespace TimeMaster
