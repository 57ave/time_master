#include "CameraManager.hpp"
#include "Config.hpp"
#include "raymath.h"
#include <cmath>
#include <cstdio>

namespace TimeMaster {

CameraManager::CameraManager() 
    : m_distance(CAMERA_DISTANCE)
    , m_height(CAMERA_HEIGHT)
    , m_angleAroundPlayer(0.0f)
    , m_pitch(20.0f)
    , m_isThirdPerson(true)  // Start in third-person by default
    , m_mouseSensitivity(0.15f)
    , m_cursorLocked(false) {
    
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
    
    // Always handle mouse input during third person mode
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
    (void)deltaTime;
    
    // GetMouseDelta() returns raw mouse movement when cursor is disabled
    Vector2 mouseDelta = GetMouseDelta();
    
    // Only take a small fraction of the delta to smooth out the movement
    // This handles the case where raylib returns very large deltas
    mouseDelta.x *= 0.01f;  // Take 1% of the delta
    mouseDelta.y *= 0.01f;
    
    // DEBUG: Print to see what's happening
    static int frameCount = 0;
    frameCount++;
    if (frameCount % 60 == 0) {
        printf("Camera: Delta=(%.2f, %.2f) | Angle=%.2f | Pitch=%.2f\n", 
               mouseDelta.x, mouseDelta.y, m_angleAroundPlayer, m_pitch);
        fflush(stdout);
    }
    
    // Apply sensitivity from config (using larger value since we scaled delta down)
    float sensitivity = GameConfig::GetInstance().mouseSensitivity * 1000.0f;

    // Update rotation angles
    // Horizontal rotation (left-right)
    m_angleAroundPlayer -= mouseDelta.x * sensitivity;
    
    // Wrap angle to keep it in a reasonable range (0-360 degrees)
    while (m_angleAroundPlayer < 0.0f) m_angleAroundPlayer += 360.0f;
    while (m_angleAroundPlayer >= 360.0f) m_angleAroundPlayer -= 360.0f;
    
    // Vertical rotation (up-down)
    m_pitch -= mouseDelta.y * sensitivity;
    
    // Clamp pitch to prevent camera flipping
    if (m_pitch < 10.0f) m_pitch = 10.0f;
    if (m_pitch > 85.0f) m_pitch = 85.0f;
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
    if (IsCursorHidden()) {
        // Unlock cursor
        EnableCursor();
        m_cursorLocked = false;
    } else {
        // Lock cursor
        DisableCursor();
        GetMouseDelta(); // Clear any accumulated mouse delta
        m_cursorLocked = true;
    }
}

bool CameraManager::IsCursorLocked() const {
    return m_cursorLocked;
}

void CameraManager::SetMouseSensitivity(float sensitivity) {
    m_mouseSensitivity = sensitivity;
}

} // namespace TimeMaster
