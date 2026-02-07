#include "Projectile.hpp"
#include "raymath.h"
#include <cmath>

namespace TimeMaster {

Projectile::Projectile() 
    : m_position{0, 0, 0}
    , m_velocity{0, 0, 0}
    , m_radius(PROJECTILE_RADIUS)
    , m_active(false)
    , m_color(ORANGE) {
}

void Projectile::Launch(Vector3 startPos, Vector3 targetPos) {
    auto& config = GameConfig::GetInstance();
    m_position = startPos;
    Vector3 direction = Vector3Subtract(targetPos, startPos);
    direction = Vector3Normalize(direction);
    m_velocity = Vector3Scale(direction, config.projectileSpeed);
    m_active = true;
}

void Projectile::Update(float deltaTime) {
    if (!m_active) return;
    
    m_position = Vector3Add(m_position, Vector3Scale(m_velocity, deltaTime));
    
    // Deactivate if out of bounds
    if (std::fabs(m_position.x) > ARENA_SIZE || 
        m_position.y < 0 || 
        m_position.y > 200 ||
        std::fabs(m_position.z) > ARENA_SIZE) {
        m_active = false;
    }
}

void Projectile::Draw() const {
    if (m_active) {
        DrawSphere(m_position, m_radius, m_color);
    }
}

void Projectile::Deactivate() {
    m_active = false;
}

bool Projectile::CheckCollision(Vector3 pos, float otherRadius) {
    if (!m_active) return false;
    
    if (Vector3Distance(m_position, pos) < m_radius + otherRadius) {
        m_active = false;
        return true;
    }
    return false;
}

} // namespace TimeMaster
