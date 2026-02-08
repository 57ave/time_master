#include "Player.hpp"
#include "raymath.h"
#include <cstdio>

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
    , m_isMoving(false) {
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
            TraceLog(LOG_INFO, "Player model loaded successfully with %d animations", s_animationCount);
            TraceLog(LOG_INFO, "Player model has %d materials", s_model.materialCount);
            
            // Debug: Check if materials have textures
            for (int i = 0; i < s_model.materialCount; i++) {
                if (s_model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id > 0) {
                    TraceLog(LOG_INFO, "Material %d has diffuse texture (id: %d)", i, 
                            s_model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id);
                } else {
                    TraceLog(LOG_WARNING, "Material %d has NO diffuse texture!", i);
                }
            }
        } else {
            TraceLog(LOG_WARNING, "Failed to load player model - using fallback rendering");
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
    m_size = {40.0f / 3.0f, 60.0f / 3.0f, 40.0f / 3.0f};  // Width, Height, Depth - Divided by 3 to make player smaller
    float halfHeight = m_size.y / 2.0f;
    m_position = {-200, halfHeight + 5.0f, 0};  // Above ground to account for arena visual thickness
    m_velocity = {0, 0, 0};  // No initial velocity
    m_speed = config.playerSpeed;
    m_time = config.playerStartingTime;
    m_isAlive = true;
    m_color = BLUE;
    
    // Reset animation state
    m_currentAnimFrame = 0;
    m_currentAnimIndex = -1;
    m_animTimer = 0.0f;
    m_isMoving = false;
}

void Player::Update(float deltaTime) {
    Vector3 movement = {0, 0, 0};
    
    // Inverted controls
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) movement.z += 1;    // W = Back
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) movement.z -= 1;  // S = Forward
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.x += 1;  // A = Right
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.x -= 1; // D = Left
    
    m_isMoving = (Vector3Length(movement) > 0);
    
    if (m_isMoving) {
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
    
    // Update animations
    if (s_modelLoaded && s_animationCount > 0) {
        // Choose animation: 0 = idle/walk (we'll use first animation)
        int animIndex = 0;
        
        if (animIndex != m_currentAnimIndex) {
            m_currentAnimIndex = animIndex;
            m_currentAnimFrame = 0;
            m_animTimer = 0.0f;
        }
        
        // Update animation frame
        if (m_currentAnimIndex >= 0 && m_currentAnimIndex < s_animationCount) {
            m_animTimer += deltaTime;
            float frameTime = 1.0f / 30.0f;  // Assume 30 FPS animation
            
            if (m_animTimer >= frameTime) {
                m_animTimer = 0.0f;
                m_currentAnimFrame++;
                
                // Loop animation
                if (m_currentAnimFrame >= s_animations[m_currentAnimIndex].frameCount) {
                    m_currentAnimFrame = 0;
                }
                
                // Apply animation frame to model
                UpdateModelAnimation(s_model, s_animations[m_currentAnimIndex], m_currentAnimFrame);
            }
        }
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
    
    if (s_modelLoaded) {
        // Calculate scale to match the hitbox size
        float scale = m_size.y;  // Use height as the primary scaling factor
        
        // Draw the 3D player model
        // Position the model at player position
        Vector3 drawPos = m_position;
        drawPos.y -= m_size.y / 2.0f;  // Adjust for model origin (base of model)
        
        // Use RAYWHITE (slightly off-white) or try without tint override
        // DrawModel uses the model's materials directly
        DrawModel(s_model, drawPos, scale, WHITE);
    } else {
        // Fallback: Draw rectangular hitbox
        DrawCube(m_position, m_size.x, m_size.y, m_size.z, m_color);
        DrawCubeWires(m_position, m_size.x, m_size.y, m_size.z, DARKBLUE);
    }
    
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
