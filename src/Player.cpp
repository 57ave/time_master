#include "Player.hpp"
#include "raymath.h"
#include <cstdio>
#include <algorithm>

constexpr float ARENA_RADIUS = 400.0f;
constexpr Vector2 ARENA_CENTER = { 60.0f, 10.0f };

namespace TimeMaster {

// Static member initialization
Model Player::s_model = {0};
bool Player::s_modelLoaded = false;
ModelAnimation* Player::s_animations = nullptr;
int Player::s_animationCount = 0;

Player::Player()
    : m_currentAnimFrame(0)
    , m_currentAnimIndex(-1)
    , m_animTimer(0.0f)
    , m_isMoving(false)
{
    Reset();
}

Player::~Player() {
    // Model is static and unloaded elsewhere
}

void Player::LoadModel() {
    if (!s_modelLoaded) {
        const char* modelPath = "assets/models/player/scene.gltf";
        s_model = ::LoadModel(modelPath);

        if (s_model.meshCount > 0 && s_model.meshes != nullptr) {
            s_animations = ::LoadModelAnimations(modelPath, &s_animationCount);
            s_modelLoaded = true;
        } else {
            s_model = (Model){0};
            s_modelLoaded = false;
        }
    }
}

void Player::UnloadModel() {
    if (s_modelLoaded) {
        ::UnloadModel(s_model);
        if (s_animations) {
            ::UnloadModelAnimations(s_animations, s_animationCount);
        }
        s_modelLoaded = false;
    }
}

void Player::Reset() {
    auto& config = GameConfig::GetInstance();

    m_size = {40.0f / 3.0f, 60.0f / 3.0f, 40.0f / 3.0f};
    float halfHeight = m_size.y / 2.0f;

    m_position = {-200.0f, halfHeight + 5.0f, 0.0f};
    m_velocity = {0, 0, 0};

    m_speed = config.playerSpeed;
    m_time = config.playerStartingTime;
    m_isAlive = true;
    m_color = BLUE;

    m_currentAnimFrame = 0;
    m_currentAnimIndex = -1;
    m_animTimer = 0.0f;
    m_isMoving = false;
}

void Player::Update(float deltaTime) {
    Vector3 movement = {0, 0, 0};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    movement.z += 1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  movement.z -= 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  movement.x += 1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.x -= 1;

    m_isMoving = (Vector3Length(movement) > 0);

    if (m_isMoving) {
        Move(Vector3Normalize(movement), deltaTime);
    }

    // Gravity
    m_velocity.y -= GRAVITY * deltaTime;
    m_position.y += m_velocity.y * deltaTime;

    float halfHeight = m_size.y / 2.0f;
    if (m_position.y - halfHeight <= 5.0f) {
        m_position.y = halfHeight + 5.0f;
        m_velocity.y = 0;
    }

    // Animation
    if (s_modelLoaded && s_animationCount > 0) {
        int animIndex = 0;

        if (animIndex != m_currentAnimIndex) {
            m_currentAnimIndex = animIndex;
            m_currentAnimFrame = 0;
            m_animTimer = 0.0f;
        }

        m_animTimer += deltaTime;
        if (m_animTimer >= (1.0f / 30.0f)) {
            m_animTimer = 0.0f;
            m_currentAnimFrame++;

            if (m_currentAnimFrame >= s_animations[m_currentAnimIndex].frameCount) {
                m_currentAnimFrame = 0;
            }

            UpdateModelAnimation(
                s_model,
                s_animations[m_currentAnimIndex],
                m_currentAnimFrame
            );
        }
    }
}

void Player::Move(Vector3 direction, float deltaTime) {
    m_position.x += direction.x * m_speed * deltaTime;
    m_position.z += direction.z * m_speed * deltaTime;

    ClampToArenaCircle();
}

void Player::ClampToArenaCircle()
{
    float playerRadius = std::max(m_size.x, m_size.z) * 0.5f;
    float maxRadius = ARENA_RADIUS - playerRadius;

    Vector2 pos2D = {
        m_position.x - ARENA_CENTER.x,
        m_position.z - ARENA_CENTER.y
    };

    float distance = Vector2Length(pos2D);

    if (distance > maxRadius) {
        Vector2 dir = Vector2Normalize(pos2D);
        Vector2 clamped = Vector2Scale(dir, maxRadius);

        m_position.x = clamped.x + ARENA_CENTER.x;
        m_position.z = clamped.y + ARENA_CENTER.y;
    }
}


AABB Player::GetAABB() const {
    Vector3 halfExtents = Vector3Scale(m_size, 0.5f);
    return AABB::FromCenter(m_position, halfExtents);
}

void Player::ApplyPushback(Vector3 pushback) {
    m_position = Vector3Add(m_position, pushback);
    ClampToArenaCircle();
}

void Player::Draw() const {
    if (!m_isAlive) return;

    if (s_modelLoaded) {
        float scale = m_size.y;

        Vector3 drawPos = m_position;
        drawPos.y -= m_size.y / 2.0f;

        DrawModel(s_model, drawPos, scale, WHITE);
    } else {
        DrawCube(m_position, m_size.x, m_size.y, m_size.z, m_color);
        DrawCubeWires(m_position, m_size.x, m_size.y, m_size.z, DARKBLUE);
    }
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
    m_time = std::min(m_time + amount, config.playerMaxTime);
}

std::string Player::GetTimeString() const {
    int minutes = static_cast<int>(m_time / 60);
    int seconds = static_cast<int>(m_time) % 60;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d:%02d", minutes, seconds);
    return std::string(buffer);
}

void Player::UpdateWithCamera(float deltaTime, Vector3 cameraForward, Vector3 cameraRight)
{
    Vector3 movement = {0, 0, 0};

    // On ignore la composante verticale de la caméra
    cameraForward.y = 0.0f;
    cameraRight.y   = 0.0f;

    cameraForward = Vector3Normalize(cameraForward);
    cameraRight   = Vector3Normalize(cameraRight);

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        movement = Vector3Add(movement, cameraForward);
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        movement = Vector3Subtract(movement, cameraForward);
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        movement = Vector3Subtract(movement, cameraRight);
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        movement = Vector3Add(movement, cameraRight);

    if (Vector3Length(movement) > 0.0f)
        Move(Vector3Normalize(movement), deltaTime);
}


} // namespace TimeMaster
