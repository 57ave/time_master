#include "CameraManager.hpp"
#include "Config.hpp"

namespace TimeMaster {

CameraManager::CameraManager() {
    Reset();
}

void CameraManager::Reset() {
    m_camera.position = {-CAMERA_DISTANCE, CAMERA_HEIGHT, CAMERA_DISTANCE};
    m_camera.target = {0.0f, 50.0f, 0.0f};
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fovy = CAMERA_FOV;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void CameraManager::Update(Vector3 targetPosition) {
    // Optional: implement camera following logic
    // For now, camera is static
    (void)targetPosition; // Suppress unused warning
}

} // namespace TimeMaster
