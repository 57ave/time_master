#include "Tomato.hpp"
#include "raymath.h"

namespace TimeMaster {

Tomato::Tomato() 
    : m_position{0, 0, 0}
    , m_radius(TOMATO_RADIUS)
    , m_lifetime(0.0f)
    , m_rotationAngle(0.0f)
    , m_active(false) {
}

void Tomato::Spawn(float x, float y, float z) {
    m_position = {x, y, z};
    m_active = true;
    m_lifetime = TOMATO_LIFETIME;
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
    
    // Draw tomato body
    DrawSphere(m_position, m_radius, RED);
    
    // Draw stem
    Vector3 stemPos = {
        m_position.x, 
        m_position.y + m_radius * 0.8f, 
        m_position.z
    };
    DrawSphere(stemPos, m_radius * 0.3f, GREEN);
}

void Tomato::OnCollect() {
    m_active = false;
}

bool Tomato::CheckCollision(Vector3 pos, float otherRadius) const {
    if (!m_active) return false;
    return Vector3Distance(m_position, pos) < m_radius + otherRadius;
}

} // namespace TimeMaster
