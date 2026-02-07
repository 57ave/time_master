#include "Boss.hpp"
#include "raymath.h"
#include <cstdio>
#include <cstdlib>

namespace TimeMaster {

Boss::Boss() {
    Reset();
}

void Boss::Reset() {
    m_position = {200, 80, 0};
    m_width = BOSS_WIDTH;
    m_height = BOSS_HEIGHT;
    m_depth = BOSS_DEPTH;
    m_time = BOSS_STARTING_TIME;
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
    if (m_position.z <= -ARENA_SIZE / 2 + m_depth / 2 || 
        m_position.z >= ARENA_SIZE / 2 - m_depth / 2) {
        m_velocity.z *= -1;
    }
    if (m_position.x <= -ARENA_SIZE / 2 + m_width / 2 || 
        m_position.x >= ARENA_SIZE / 2 - m_width / 2) {
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
    
    // Draw main body
    DrawCube(m_position, m_width, m_height, m_depth, m_color);
    DrawCubeWires(m_position, m_width, m_height, m_depth, DARKBROWN);
    
    // Draw eyes
    Vector3 leftEye = {
        m_position.x - m_width * 0.2f, 
        m_position.y + m_height * 0.2f, 
        m_position.z - m_depth * 0.51f
    };
    Vector3 rightEye = {
        m_position.x + m_width * 0.2f, 
        m_position.y + m_height * 0.2f, 
        m_position.z - m_depth * 0.51f
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
    float random = static_cast<float>(GetRandomValue(0, 100)) / 100.0f;
    m_attackCooldown = BOSS_ATTACK_COOLDOWN_MIN + 
                       random * (BOSS_ATTACK_COOLDOWN_MAX - BOSS_ATTACK_COOLDOWN_MIN);
}

BoundingBox Boss::GetBoundingBox() const {
    return {
        {m_position.x - m_width / 2, m_position.y - m_height / 2, m_position.z - m_depth / 2},
        {m_position.x + m_width / 2, m_position.y + m_height / 2, m_position.z + m_depth / 2}
    };
}

bool Boss::CheckCollisionWithPlayer(const Player& player) const {
    return CheckCollisionBoxSphere(GetBoundingBox(), player.GetPosition(), player.GetRadius());
}

std::string Boss::GetTimeString() const {
    int minutes = static_cast<int>(m_time / 60);
    int seconds = static_cast<int>(m_time) % 60;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d:%02d", minutes, seconds);
    return std::string(buffer);
}

} // namespace TimeMaster
