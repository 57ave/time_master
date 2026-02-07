#include "Boss.hpp"
#include "Player.hpp"
#include "raymath.h"
#include <cstdio>
#include <cstdlib>

namespace TimeMaster {

Boss::Boss() {
    Reset();
}

void Boss::Reset() {
    auto& config = GameConfig::GetInstance();
    m_position = {200, 80, 0};
    m_size = {BOSS_WIDTH, BOSS_HEIGHT, BOSS_DEPTH};  // Use Vector3 for size
    m_time = config.bossStartingTime;
    m_isAlive = true;
    m_attackCooldown = 0.0f;
    m_moveTimer = 0.0f;
    m_velocity = {0, 0, 30};
    m_color = RED;
}

void Boss::Update(float deltaTime) {
    if (!m_isAlive) return;
    
    m_moveTimer += deltaTime;
    m_position = Vector3Add(m_position, Vector3Scale(m_velocity, deltaTime));
    
    // Bounce off arena boundaries
    if (m_position.z <= -ARENA_SIZE / 2 + m_size.z / 2 || 
        m_position.z >= ARENA_SIZE / 2 - m_size.z / 2) {
        m_velocity.z *= -1;
    }
    if (m_position.x <= -ARENA_SIZE / 2 + m_size.x / 2 || 
        m_position.x >= ARENA_SIZE / 2 - m_size.x / 2) {
        m_velocity.x *= -1;
    }
    
    // Change direction occasionally
    if (m_moveTimer > 2.0f) {
        m_moveTimer = 0;
        if (GetRandomValue(0, 1)) {
            m_velocity.z *= -1;
        }
        if (GetRandomValue(0, 1)) {
            m_velocity.x = static_cast<float>(GetRandomValue(-20, 20));
        }
    }
    
    m_attackCooldown -= deltaTime;
}

void Boss::Draw() const {
    if (!m_isAlive) return;
    
    // Draw main body using m_size
    DrawCube(m_position, m_size.x, m_size.y, m_size.z, m_color);
    DrawCubeWires(m_position, m_size.x, m_size.y, m_size.z, DARKBROWN);
    
    // Draw eyes
    Vector3 leftEye = {
        m_position.x - m_size.x * 0.2f, 
        m_position.y + m_size.y * 0.2f, 
        m_position.z - m_size.z * 0.51f
    };
    Vector3 rightEye = {
        m_position.x + m_size.x * 0.2f, 
        m_position.y + m_size.y * 0.2f, 
        m_position.z - m_size.z * 0.51f
    };
    
    DrawSphere(leftEye, 5.0f, BLACK);
    DrawSphere(rightEye, 5.0f, BLACK);
    DrawSphere(leftEye, 2.5f, YELLOW);
    DrawSphere(rightEye, 2.5f, YELLOW);
}

void Boss::TakeDamage(float damage) {
    m_time -= damage;
    if (m_time <= 0) {
        m_time = 0;
        m_isAlive = false;
    }
}

bool Boss::CanAttack() const {
    return m_attackCooldown <= 0;
}

void Boss::ResetAttackCooldown() {
    auto& config = GameConfig::GetInstance();
    float random = static_cast<float>(GetRandomValue(0, 100)) / 100.0f;
    m_attackCooldown = config.bossAttackCooldownMin + 
                       random * (config.bossAttackCooldownMax - config.bossAttackCooldownMin);
}

AABB Boss::GetAABB() const {
    Vector3 halfExtents = Vector3Scale(m_size, 0.5f);
    return AABB::FromCenter(m_position, halfExtents);
}

void Boss::ApplyPushback(Vector3 pushback) {
    m_position = Vector3Add(m_position, pushback);
    
    // Keep in bounds after pushback
    float halfWidth = m_size.x / 2.0f;
    float halfDepth = m_size.z / 2.0f;
    
    m_position.x = Clamp(m_position.x, -ARENA_SIZE / 2 + halfWidth, ARENA_SIZE / 2 - halfWidth);
    m_position.z = Clamp(m_position.z, -ARENA_SIZE / 2 + halfDepth, ARENA_SIZE / 2 - halfDepth);
}

bool Boss::CheckCollisionWithPlayer(const Player& player) const {
    return GetAABB().Intersects(player.GetAABB());
}

std::string Boss::GetTimeString() const {
    int minutes = static_cast<int>(m_time / 60);
    int seconds = static_cast<int>(m_time) % 60;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d:%02d", minutes, seconds);
    return std::string(buffer);
}

} // namespace TimeMaster
