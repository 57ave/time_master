#include "Player.hpp"
#include "raymath.h"
#include <cstdio>

namespace TimeMaster {

Player::Player() {
    Reset();
}

void Player::Reset() {
    m_position = {-200, 30, 0};
    m_radius = PLAYER_RADIUS;
    m_speed = PLAYER_SPEED;
    m_time = PLAYER_STARTING_TIME;
    m_isAlive = true;
    m_color = BLUE;
}

void Player::Update(float deltaTime) {
    Vector3 movement = {0, 0, 0};
    
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) movement.z -= 1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) movement.z += 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.x -= 1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.x += 1;
    
    if (Vector3Length(movement) > 0) {
        Move(Vector3Normalize(movement), deltaTime);
    }
}

void Player::Move(Vector3 direction, float deltaTime) {
    m_position = Vector3Add(m_position, Vector3Scale(direction, m_speed * deltaTime));
    
    // Keep player in bounds
    m_position.x = Clamp(m_position.x, -ARENA_SIZE + m_radius, ARENA_SIZE - m_radius);
    m_position.z = Clamp(m_position.z, -ARENA_SIZE + m_radius, ARENA_SIZE - m_radius);
    m_position.y = 30.0f; // Keep at constant height
}

void Player::Draw() const {
    if (!m_isAlive) return;
    
    DrawSphere(m_position, m_radius, m_color);
    DrawSphereWires(m_position, m_radius + 1, 8, 8, DARKBLUE);
}

void Player::TakeDamage(float damage) {
    m_time -= damage;
    if (m_time <= 0) {
        m_time = 0;
        m_isAlive = false;
    }
}

void Player::Heal(float amount) {
    m_time += amount;
    if (m_time > PLAYER_MAX_TIME) {
        m_time = PLAYER_MAX_TIME;
    }
}

std::string Player::GetTimeString() const {
    int minutes = static_cast<int>(m_time / 60);
    int seconds = static_cast<int>(m_time) % 60;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d:%02d", minutes, seconds);
    return std::string(buffer);
}

} // namespace TimeMaster
