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
    , m_isMoving(false)
    , m_rotationAngle(0.0f) {
    Reset();
}

Player::~Player() {
    // Model is static and unloaded elsewhere
}

void Player::LoadModel() {
    if (!s_modelLoaded) {
        const char* modelPath = "assets/models/player/source/model.gltf";
        s_model = ::LoadModel(modelPath);
        if (s_model.meshCount > 0 && s_model.meshes != nullptr) {
            s_animations = ::LoadModelAnimations(modelPath, &s_animationCount);
            s_modelLoaded = true;
            TraceLog(LOG_INFO, "Player model loaded successfully with %d animations", s_animationCount);
            TraceLog(LOG_INFO, "Player model has %d materials", s_model.materialCount);
            
            // The GLTF has embedded texture data, but we need to set the texture filter
            // Debug: Check if materials have textures loaded from GLTF
            for (int i = 0; i < s_model.materialCount; i++) {
                if (s_model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id > 0) {
                    TraceLog(LOG_INFO, "Material %d has diffuse texture (id: %d)", i, 
                            s_model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id);
                    // Set texture filter for pixel-art style
                    SetTextureFilter(s_model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture, TEXTURE_FILTER_BILINEAR);
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
    m_rotationAngle = 0.0f;
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
        // Choose animation based on movement (0 = first animation, typically idle/walk)
        int animIndex = 0;
        bool shouldLoop = true;  // Always loop player animations
        
        // Make sure animIndex is valid
        if (animIndex >= s_animationCount) {
            animIndex = 0;
        }
        
        // Reset animation timer if we changed animation
        if (m_currentAnimIndex != animIndex) {
            m_currentAnimIndex = animIndex;
            m_animTimer = 0.0f;
            m_currentAnimFrame = 0;
        }
        
        // Calculate animation frame based on time
        // GLTF animations are typically exported at 60 FPS
        float animFPS = 60.0f;
        m_animTimer += deltaTime;
        
        if (shouldLoop) {
            // Loop animation
            float animDuration = s_animations[animIndex].frameCount / animFPS;
            while (m_animTimer >= animDuration) {
                m_animTimer -= animDuration;
            }
        }
        
        // Calculate current frame from timer
        m_currentAnimFrame = (int)(m_animTimer * animFPS);
        
        // Clamp to valid frame range
        if (m_currentAnimFrame >= s_animations[animIndex].frameCount) {
            m_currentAnimFrame = s_animations[animIndex].frameCount - 1;
        }
        
        // Update the model animation
        UpdateModelAnimation(s_model, s_animations[animIndex], m_currentAnimFrame);
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
        // Get model bounds to calculate proper positioning
        BoundingBox bounds = GetModelBoundingBox(s_model);
        
        // Use a fixed uniform scale for the player model
        float uniformScale = 10.0f;  // Adjust this value to match desired size
        Vector3 modelScale = {uniformScale, uniformScale, uniformScale};
        
        // Adjust position to place model on ground
        Vector3 drawPosition = m_position;
        
        // Calculate where the bottom of the scaled model would be relative to its center
        float scaledModelBottom = bounds.min.y * uniformScale;
        
        // The model should be drawn so its bottom is at the player's position
        // Account for arena visual thickness (y=5.0)
        drawPosition.y = -scaledModelBottom + 5.0f;
        
        // Keep X and Z from m_position for horizontal placement
        drawPosition.x = m_position.x;
        drawPosition.z = m_position.z;
        
        // Draw model with rotation to face camera
        // Add 180 degrees to flip the model to face the correct direction
        DrawModelEx(
            s_model,
            drawPosition,
            {0.0f, 1.0f, 0.0f},  // Rotate around Y axis
            m_rotationAngle + 180.0f,      // Rotation angle to face camera (flipped)
            modelScale,
            WHITE
        );
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
