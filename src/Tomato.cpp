#include "Tomato.hpp"
#include "raymath.h"

namespace TimeMaster {

// Static member initialization
Model Tomato::s_model = {0};
bool Tomato::s_modelLoaded = false;

Tomato::Tomato() 
    : m_position{0, 0, 0}
    , m_radius(TOMATO_RADIUS)
    , m_lifetime(0.0f)
    , m_rotationAngle(0.0f)
    , m_active(false) {
}

void Tomato::LoadModel() {
    if (!s_modelLoaded) {
        s_model = ::LoadModel("assets/models/tomato/scene.gltf");
        // Check if model loaded successfully with valid meshes and materials
        if (s_model.meshCount > 0 && s_model.meshes != nullptr) {
            s_modelLoaded = true;
            TraceLog(LOG_INFO, "Tomato model loaded successfully");
        } else {
            TraceLog(LOG_WARNING, "Failed to load tomato model - using fallback rendering");
            // Ensure the model is zeroed out if it failed to load
            s_model = (Model){0};
            s_modelLoaded = false;
        }
    }
}

void Tomato::UnloadModel() {
    if (s_modelLoaded) {
        ::UnloadModel(s_model);
        s_modelLoaded = false;
    }
}

void Tomato::Spawn(float x, float y, float z) {
    auto& config = GameConfig::GetInstance();
    m_position = {x, y, z};
    m_active = true;
    m_lifetime = config.tomatoLifetime;
    m_rotationAngle = 0.0f;
}

void Tomato::Update(float deltaTime) {
    if (!m_active) return;
    
    m_lifetime -= deltaTime;
    m_rotationAngle += deltaTime * 90.0f;
    
    if (m_lifetime <= 0) {
        m_active = false;
    }
}

void Tomato::Draw() const {
    if (!m_active) return;
    
    if (s_modelLoaded) {
        DrawModelEx(s_model, m_position, {0, 1, 0}, m_rotationAngle, {m_radius, m_radius, m_radius}, WHITE);
    } else {
        // Fallback to sphere if model not loaded
        DrawSphere(m_position, m_radius, RED);
        
        // Draw stem
        Vector3 stemPos = {
            m_position.x, 
            m_position.y + m_radius * 0.8f, 
            m_position.z
        };
        DrawSphere(stemPos, m_radius * 0.3f, GREEN);
    }
}

void Tomato::OnCollect() {
    m_active = false;
}

bool Tomato::CheckCollision(Vector3 pos, float otherRadius) const {
    if (!m_active) return false;
    return Vector3Distance(m_position, pos) < m_radius + otherRadius;
}

} // namespace TimeMaster
