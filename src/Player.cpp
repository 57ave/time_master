#include "Player.hpp"
#include "raymath.h"
#include <cstdio>
#include <algorithm>
#include <cstring>

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
<<<<<<< Updated upstream
    , m_isRunning(false)
    , m_rotationAngle(0.0f)
{
=======
    , m_rotationAngle(0.0f)
    , m_isDashing(false)
    , m_dashTimer(0.0f)
    , m_dashCooldownTimer(0.0f)
    , m_dashDirection({0, 0, 0})
    , m_stamina(MAX_STAMINA) {
>>>>>>> Stashed changes
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

            TraceLog(LOG_INFO, "Player model loaded with %d animations", s_animationCount);

            for (int i = 0; i < s_animationCount; i++) {
                TraceLog(LOG_INFO, "  Animation %d: %s (%d frames)",
                         i, s_animations[i].name, s_animations[i].frameCount);
            }

            for (int i = 0; i < s_model.materialCount; i++) {
                if (s_model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id > 0) {
                    SetTextureFilter(
                        s_model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture,
                        TEXTURE_FILTER_BILINEAR
                    );
                }
            }
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
    m_isRunning = false;
    m_rotationAngle = 0.0f;
    
    // Reset dash state
    m_isDashing = false;
    m_dashTimer = 0.0f;
    m_dashCooldownTimer = 0.0f;
    m_dashDirection = {0, 0, 0};
    m_stamina = MAX_STAMINA;
}

void Player::Update(float deltaTime) {
    // Update dash timers
    if (m_isDashing) {
        m_dashTimer -= deltaTime;
        if (m_dashTimer <= 0.0f) {
            m_isDashing = false;
            m_dashTimer = 0.0f;  // Ensure timer is exactly 0
            m_dashCooldownTimer = DASH_COOLDOWN;
            m_dashDirection = {0, 0, 0};  // Clear dash direction
        }
    }
    
    if (m_dashCooldownTimer > 0.0f) {
        m_dashCooldownTimer -= deltaTime;
        if (m_dashCooldownTimer < 0.0f) {
            m_dashCooldownTimer = 0.0f;  // Ensure it doesn't go negative
        }
    }
    
    // Regenerate stamina when not dashing
    if (!m_isDashing && m_stamina < MAX_STAMINA) {
        m_stamina += STAMINA_REGEN_RATE * deltaTime;
        if (m_stamina > MAX_STAMINA) {
            m_stamina = MAX_STAMINA;
        }
    }
    
    Vector3 movement = {0, 0, 0};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    movement.z += 1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  movement.z -= 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  movement.x += 1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.x -= 1;

    m_isMoving = (Vector3Length(movement) > 0);
<<<<<<< Updated upstream

    if (m_isMoving) {
=======
    
    // Handle dash movement separately
    if (m_isDashing) {
        // Move in dash direction with flat speed boost
        float dashSpeed = m_speed + DASH_SPEED_BOOST;
        Vector3 dashMovement = Vector3Scale(m_dashDirection, dashSpeed * deltaTime);
        m_position = Vector3Add(m_position, dashMovement);
        
        // Debug output
        TraceLog(LOG_INFO, "DASH: speed=%f, boost=%f, total=%f, timer=%f", 
                 m_speed, DASH_SPEED_BOOST, dashSpeed, m_dashTimer);
    } else if (m_isMoving) {
>>>>>>> Stashed changes
        Move(Vector3Normalize(movement), deltaTime);
        m_rotationAngle = atan2f(movement.x, movement.z) * RAD2DEG;
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
        static int idleAnim = -1;
        static int walkAnim = -1;
        static int runAnim  = -1;

        if (idleAnim == -1) {
            for (int i = 0; i < s_animationCount; i++) {
                const char* name = s_animations[i].name;
                if (strstr(name, "Idle") || strstr(name, "idle")) idleAnim = i;
                if (strstr(name, "Walk") || strstr(name, "walk")) walkAnim = i;
                if (strstr(name, "Run")  || strstr(name, "run"))  runAnim  = i;
            }
        }

        int animIndex = (idleAnim >= 0) ? idleAnim : 0;

        if (m_isMoving) {
            if (m_isRunning && runAnim >= 0) animIndex = runAnim;
            else if (walkAnim >= 0) animIndex = walkAnim;
        }

        if (animIndex >= s_animationCount) animIndex = 0;

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

void Player::ClampToArenaCircle() {
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
        BoundingBox bounds = GetModelBoundingBox(s_model);

        float scale = 10.0f;
        Vector3 modelScale = {scale, scale, scale};

        Vector3 drawPos = m_position;
        drawPos.y = -bounds.min.y * scale + 5.0f;

        DrawModelEx(
            s_model,
            drawPos,
            {0.0f, 1.0f, 0.0f},
            m_rotationAngle + 180.0f,
            modelScale,
            WHITE
        );
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

<<<<<<< Updated upstream
void Player::UpdateWithCamera(float deltaTime, Vector3 cameraForward, Vector3 cameraRight)
{
=======
void Player::UpdateWithCamera(float deltaTime, Vector3 cameraForward, Vector3 cameraRight) {
    // Don't process normal movement if dashing
    if (m_isDashing) {
        return;  // Dash movement is handled in Update()
    }
    
>>>>>>> Stashed changes
    Vector3 movement = {0, 0, 0};

    cameraForward.y = 0.0f;
    cameraRight.y   = 0.0f;

    cameraForward = Vector3Normalize(cameraForward);
    cameraRight   = Vector3Normalize(cameraRight);

    bool forward  = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
    bool backward = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
    bool left     = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    bool right    = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

    m_isRunning = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    if (forward)  movement = Vector3Add(movement, cameraForward);
    if (backward) movement = Vector3Subtract(movement, cameraForward);
    if (left)     movement = Vector3Subtract(movement, cameraRight);
    if (right)    movement = Vector3Add(movement, cameraRight);

    if (Vector3Length(movement) > 0.0f) {
        float originalSpeed = m_speed;
        m_speed = m_isRunning ? m_speed * 1.8f : m_speed;

        Move(Vector3Normalize(movement), deltaTime);

        m_speed = originalSpeed;
        m_rotationAngle = atan2f(movement.x, movement.z) * RAD2DEG;
    }
}

void Player::StartDash(Vector3 direction) {
    if (!CanDash() || Vector3Length(direction) == 0) return;
    
    TraceLog(LOG_INFO, "StartDash called: stamina=%f, duration=%f, boost=%f", 
             m_stamina, DASH_DURATION, DASH_SPEED_BOOST);
    
    // Check stamina
    if (m_stamina >= DASH_STAMINA_COST) {
        // Normal dash - consume stamina
        m_stamina -= DASH_STAMINA_COST;
        m_isDashing = true;
        m_dashTimer = DASH_DURATION;
        m_dashDirection = Vector3Normalize(direction);
    } else if (m_stamina < DASH_STAMINA_COST) {
        // Not enough stamina - consume time instead
        m_time -= STAMINA_DEPLETED_TIME_COST;
        if (m_time < 0) m_time = 0;
        
        // Still allow the dash but use remaining stamina
        m_stamina = 0;
        m_isDashing = true;
        m_dashTimer = DASH_DURATION;
        m_dashDirection = Vector3Normalize(direction);
    }
}

} // namespace TimeMaster
