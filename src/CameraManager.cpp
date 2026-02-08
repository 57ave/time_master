#include "CameraManager.hpp"
#include "Config.hpp"
#include "raymath.h"
#include <cmath>
#include <cstdio>

constexpr float ARENA_RADIUS = 375.0f;
constexpr Vector2 ARENA_CENTER = { 60.0f, 10.0f };

namespace TimeMaster {

CameraManager::CameraManager() 
    : m_distance(CAMERA_DISTANCE)
    , m_height(CAMERA_HEIGHT)
    , m_angleAroundPlayer(0.0f)
    , m_pitch(20.0f)
    , m_isThirdPerson(true)
    , m_mouseSensitivity(0.15f)
    , m_cursorLocked(false)
{
    Reset();
}

void CameraManager::Reset() {
    if (m_isThirdPerson) {
        m_camera.position = {0.0f, m_height, m_distance};
        m_camera.target = {0.0f, 50.0f, 0.0f};
    } else {
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
        return;
    }

    HandleMouseInput(deltaTime);

    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        AdjustDistance(-wheel * 20.0f);
    }

    float horizontalDistance = m_distance * cosf(m_pitch * DEG2RAD);
    float verticalDistance   = m_distance * sinf(m_pitch * DEG2RAD);

    float offsetX = horizontalDistance * sinf(m_angleAroundPlayer * DEG2RAD);
    float offsetZ = horizontalDistance * cosf(m_angleAroundPlayer * DEG2RAD);

    m_camera.position.x = playerPosition.x - offsetX;
    m_camera.position.y = playerPosition.y + verticalDistance;
    m_camera.position.z = playerPosition.z - offsetZ;

    // 🔒 Clamp caméra dans la map
    ClampCameraToArena();

    // La caméra regarde toujours le joueur
    m_camera.target = playerPosition;
    m_camera.target.y += 30.0f;
}

void CameraManager::ClampCameraToArena()
{
    // Position caméra relative au centre réel de l'arène
    Vector2 camPos2D = {
        m_camera.position.x - ARENA_CENTER.x,
        m_camera.position.z - ARENA_CENTER.y
    };

    float distance = Vector2Length(camPos2D);

    float maxRadius = ARENA_RADIUS - 1.0f;

    if (distance > maxRadius) {
        Vector2 dir = Vector2Normalize(camPos2D);
        Vector2 clamped = Vector2Scale(dir, maxRadius);

        // Retour en coordonnées monde
        m_camera.position.x = clamped.x + ARENA_CENTER.x;
        m_camera.position.z = clamped.y + ARENA_CENTER.y;
    }
}

void CameraManager::HandleMouseInput(float deltaTime) {
    (void)deltaTime;

    Vector2 mouseDelta = GetMouseDelta();
    mouseDelta.x *= 0.01f;
    mouseDelta.y *= 0.01f;

    static int frameCount = 0;
    frameCount++;
    if (frameCount % 60 == 0) {
        printf("Camera: Delta=(%.2f, %.2f) | Angle=%.2f | Pitch=%.2f\n", 
               mouseDelta.x, mouseDelta.y, m_angleAroundPlayer, m_pitch);
        fflush(stdout);
    }

    float sensitivity = GameConfig::GetInstance().mouseSensitivity * 1000.0f;

    m_angleAroundPlayer -= mouseDelta.x * sensitivity;

    while (m_angleAroundPlayer < 0.0f)   m_angleAroundPlayer += 360.0f;
    while (m_angleAroundPlayer >= 360.0f) m_angleAroundPlayer -= 360.0f;

    m_pitch -= mouseDelta.y * sensitivity;
    if (m_pitch < 10.0f) m_pitch = 10.0f;
    if (m_pitch > 85.0f) m_pitch = 85.0f;
}

Vector3 CameraManager::GetForwardDirection() const {
    Vector3 forward;
    forward.x = sinf(m_angleAroundPlayer * DEG2RAD);
    forward.y = 0.0f;
    forward.z = cosf(m_angleAroundPlayer * DEG2RAD);
    return Vector3Normalize(forward);
}

Vector3 CameraManager::GetRightDirection() const {
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

    if (m_distance < 100.0f) m_distance = 100.0f;
    if (m_distance > 800.0f) m_distance = 800.0f;
}

void CameraManager::ToggleCursorLock() {
    if (IsCursorHidden()) {
        EnableCursor();
        m_cursorLocked = false;
    } else {
        DisableCursor();
        GetMouseDelta();
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
