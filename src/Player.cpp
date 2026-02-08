#include "Player.hpp"
#include "raymath.h"
#include <cstdio>

namespace TimeMaster {

Player::Player() {
    Reset();
}

void Player::Reset() {
    auto& config = GameConfig::GetInstance();
    m_size = {40.0f / 3.0f, 60.0f / 3.0f, 40.0f / 3.0f};  // Width, Height, Depth - Divided by 3 to make player smaller
    float halfHeight = m_size.y / 2.0f;
    m_position = {-200, halfHeight + 5.0f, 0};  // Above ground to account for arena visual thickness
    m_velocity = {0, 0, 0};  // No initial velocity
    m_speed = config.playerSpeed;
    m_time = config.playerStartingTime;
    m_isAlive = true;
    m_color = BLUE;
}

void Player::Update(float deltaTime) {
    Vector3 movement = {0, 0, 0};
    
    // Inverted controls
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) movement.z += 1;    // W = Back
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) movement.z -= 1;  // S = Forward
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.x += 1;  // A = Right
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.x -= 1; // D = Left
    
    if (Vector3Length(movement) > 0) {
        Move(Vector3Normalize(movement), deltaTime);
    }
    
    // Apply gravity
    m_velocity.y -= GRAVITY * deltaTime;
    m_position.y += m_velocity.y * deltaTime;
    
    // Check collision with arena floor
    float halfHeight = m_size.y / 2.0f;
    float bottomY = m_position.y - halfHeight;
    
    if (bottomY <= 5.0f) {
        // Player hit the ground (above y=0 to account for arena visual thickness)
        m_position.y = halfHeight + 5.0f;
        m_velocity.y = 0;  // Stop falling
    }
}

void Player::Move(Vector3 direction, float deltaTime) {
    // Only move on horizontal plane (X and Z)
    m_position.x += direction.x * m_speed * deltaTime;
    m_position.z += direction.z * m_speed * deltaTime;
    
    // Keep player in bounds (consider half-width)
    float halfWidth = m_size.x / 2.0f;
    float halfDepth = m_size.z / 2.0f;
    
    m_position.x = Clamp(m_position.x, -ARENA_SIZE + halfWidth, ARENA_SIZE - halfWidth);
    m_position.z = Clamp(m_position.z, -ARENA_SIZE + halfDepth, ARENA_SIZE - halfDepth);
    // Y position is now controlled by gravity in Update()
}

AABB Player::GetAABB() const {
    Vector3 halfExtents = Vector3Scale(m_size, 0.5f);
    return AABB::FromCenter(m_position, halfExtents);
}

void Player::ApplyPushback(Vector3 pushback) {
    m_position = Vector3Add(m_position, pushback);
    
    // Keep in bounds after pushback
    float halfWidth = m_size.x / 2.0f;
    float halfDepth = m_size.z / 2.0f;
    
    m_position.x = Clamp(m_position.x, -ARENA_SIZE + halfWidth, ARENA_SIZE - halfWidth);
    m_position.z = Clamp(m_position.z, -ARENA_SIZE + halfDepth, ARENA_SIZE - halfDepth);
}

void Player::Draw() const {
    if (!m_isAlive) return;
    
    // Draw rectangular hitbox instead of sphere
    DrawCube(m_position, m_size.x, m_size.y, m_size.z, m_color);
    DrawCubeWires(m_position, m_size.x, m_size.y, m_size.z, DARKBLUE);
    
    // Optional: Draw AABB debug
    // GetAABB().DrawDebug(GREEN);
}

void Player::TakeDamage(float damage) {
    m_time -= damage;
    if (m_time <= 0) {
        m_time = 0;
        m_isAlive = false;
    }
}

void Player::Heal(float amount) {
    auto& config = GameConfig::GetInstance();
    m_time += amount;
    if (m_time > config.playerMaxTime) {
        m_time = config.playerMaxTime;
    }
}

std::string Player::GetTimeString() const {
    int minutes = static_cast<int>(m_time / 60);
    int seconds = static_cast<int>(m_time) % 60;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d:%02d", minutes, seconds);
    return std::string(buffer);
}

void Player::UpdateWithCamera(float deltaTime, Vector3 cameraForward, Vector3 cameraRight) {
    Vector3 movement = {0, 0, 0};
    
    // Get input
    bool moveForward = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
    bool moveBackward = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
    bool moveLeft = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    bool moveRight = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
    
    // Build movement vector relative to camera
    if (moveForward) {
        movement = Vector3Add(movement, cameraForward);
    }
    if (moveBackward) {
        movement = Vector3Subtract(movement, cameraForward);
    }
    if (moveLeft) {
        movement = Vector3Subtract(movement, cameraRight);
    }
    if (moveRight) {
        movement = Vector3Add(movement, cameraRight);
    }
    
    // Apply movement if any input detected
    if (Vector3Length(movement) > 0) {
        Move(Vector3Normalize(movement), deltaTime);
    }
}

} // namespace TimeMaster
