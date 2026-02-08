#include "Boss.hpp"
#include "Player.hpp"
#include "raymath.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>

namespace TimeMaster {

Boss::Boss() 
    : m_targetRotation(0.0f)
    , m_currentRotation(0.0f)
    , m_rotationSpeed(3.0f)
    , m_currentState(BossState::IDLE)
    , m_stateTimer(0.0f)
    , m_hasAttackedInState(false)
    , m_animations(nullptr)
    , m_animationCount(0)
    , m_currentAnimFrame(0)
    , m_currentAnimIndex(-1)
    , m_animTimer(0.0f)
    , m_modelLoaded(false)
    , m_showDebugHitbox(true) {
    
    LoadModel();
    Reset();
}

Boss::~Boss() {
    UnloadModel();
}

void Boss::LoadModel() {
    const char* modelPath = "assets/models/plant_boss/scene.gltf";
    
    if (FileExists(modelPath)) {
        m_model = ::LoadModel(modelPath);  // Use global Raylib function
        m_animations = ::LoadModelAnimations(modelPath, &m_animationCount);
        m_modelLoaded = true;
        printf("Boss model loaded successfully!\n");
        printf("  Animations found: %d\n", m_animationCount);
        
        // Print info about each animation
        for (int i = 0; i < m_animationCount; i++) {
            printf("  Animation %d: %d frames\n", i, m_animations[i].frameCount);
        }
        
        // Print model bounds for debugging
        BoundingBox bounds = GetModelBoundingBox(m_model);
        printf("  Model bounds: min(%.2f, %.2f, %.2f) max(%.2f, %.2f, %.2f)\n",
               bounds.min.x, bounds.min.y, bounds.min.z,
               bounds.max.x, bounds.max.y, bounds.max.z);
        Vector3 modelSize = {
            bounds.max.x - bounds.min.x,
            bounds.max.y - bounds.min.y,
            bounds.max.z - bounds.min.z
        };
        printf("  Model size: (%.2f, %.2f, %.2f)\n", modelSize.x, modelSize.y, modelSize.z);
        printf("  Using fixed scale: 12.0 (hitbox is for collision only, not visual sizing)\n");
    } else {
        printf("Warning: Boss model not found at %s\n", modelPath);
        m_modelLoaded = false;
    }
}

void Boss::UnloadModel() {
    if (m_modelLoaded) {
        ::UnloadModel(m_model);  // Use global Raylib function
        if (m_animations) {
            ::UnloadModelAnimations(m_animations, m_animationCount);
        }
    }
}

void Boss::Reset() {
    auto& config = GameConfig::GetInstance();
    // Reduced hitbox to match visual scale better
    m_size = {BOSS_WIDTH * 0.8f, BOSS_HEIGHT * 0.8f, BOSS_DEPTH * 0.8f};
    float halfHeight = m_size.y / 2.0f;
    m_position = {200, halfHeight + 5.0f, 0};  // Above ground to avoid visual collision with arena thickness
    m_time = config.bossStartingTime;
    m_isAlive = true;
    m_attackCooldown = 0.0f;
    m_moveTimer = 0.0f;
    m_velocity = {0, 0, 0};  // Plant boss is stationary
    m_color = GREEN;
    m_currentRotation = 0.0f;
    m_targetRotation = 0.0f;
    m_currentState = BossState::IDLE;
    m_stateTimer = 0.0f;
    m_currentAnimFrame = 0;
    m_currentAnimIndex = -1;
    m_animTimer = 0.0f;
}

void Boss::Update(float deltaTime) {
    if (!m_isAlive) return;
    
    // Update state machine
    UpdateState(deltaTime);
    
    // Update attack cooldown (legacy, not used anymore)
    m_attackCooldown -= deltaTime;
    
    // Update animation frames based on current state
    if (m_modelLoaded && m_animationCount > 0) {
        // Choose which animation to play based on state
        // Animations in GLTF: 0=Attack1, 1=Attack2, 2=Attack3, 3=Defense1, 4=Defense2, 5=Defense3, 6=Walk
        int animIndex = 6; // Default to Walk (as idle)
        bool shouldLoop = false;
        
        switch (m_currentState) {
            case BossState::IDLE:
                animIndex = 6; // Walk animation serves as idle
                shouldLoop = true; // Loop idle animation
                break;
                
            case BossState::ATTACK_1:
                animIndex = 0; // Attack1
                shouldLoop = false;
                break;
                
            case BossState::ATTACK_2:
                animIndex = 1; // Attack2
                shouldLoop = false;
                break;
                
            case BossState::ATTACK_3:
                animIndex = 2; // Attack3
                shouldLoop = false;
                break;
                
            case BossState::DEATH:
                // Use Defense3 as death animation (longest one)
                animIndex = 5;
                shouldLoop = false;
                break;
        }
        
        // Make sure animIndex is valid
        if (animIndex >= m_animationCount) {
            animIndex = 0;
        }
        
        // Reset animation timer if we changed animation
        if (m_currentAnimIndex != animIndex) {
            m_currentAnimIndex = animIndex;
            m_animTimer = 0.0f;
            m_currentAnimFrame = 0;
        }
        
        // Calculate animation frame based on time
        // Get the FPS of this animation (assuming 60 FPS as standard for GLTF)
        float animFPS = 60.0f; // GLTF animations are typically exported at 60 FPS
        m_animTimer += deltaTime;
        
        if (shouldLoop) {
            // Loop animation
            float animDuration = m_animations[animIndex].frameCount / animFPS;
            while (m_animTimer >= animDuration) {
                m_animTimer -= animDuration;
            }
        } else {
            // Clamp to animation duration for non-looping animations
            float animDuration = m_animations[animIndex].frameCount / animFPS;
            if (m_animTimer > animDuration) {
                m_animTimer = animDuration - 0.001f; // Stay just before the end
            }
        }
        
        // Calculate current frame from timer
        m_currentAnimFrame = (int)(m_animTimer * animFPS);
        
        // Clamp to valid frame range
        if (m_currentAnimFrame >= m_animations[animIndex].frameCount) {
            m_currentAnimFrame = m_animations[animIndex].frameCount - 1;
        }
        
        // Update the model animation
        UpdateModelAnimation(m_model, m_animations[animIndex], m_currentAnimFrame);
    }
}

void Boss::UpdateWithPlayer(Vector3 playerPosition, float deltaTime) {
    Update(deltaTime);
    UpdateRotation(playerPosition, deltaTime);
}

void Boss::UpdateRotation(Vector3 playerPosition, float deltaTime) {
    // Calculate direction to player (only on horizontal plane)
    Vector3 directionToPlayer = Vector3Subtract(playerPosition, m_position);
    directionToPlayer.y = 0;  // Ignore vertical component
    
    if (Vector3Length(directionToPlayer) > 0.1f) {
        // Calculate target angle using atan2
        m_targetRotation = atan2f(directionToPlayer.x, directionToPlayer.z) * RAD2DEG;
        
        // Smooth rotation interpolation
        float angleDiff = m_targetRotation - m_currentRotation;
        
        // Normalize angle difference to [-180, 180]
        while (angleDiff > 180.0f) angleDiff -= 360.0f;
        while (angleDiff < -180.0f) angleDiff += 360.0f;
        
        // Smoothly interpolate
        m_currentRotation += angleDiff * m_rotationSpeed * deltaTime;
        
        // Keep rotation in [0, 360] range
        while (m_currentRotation > 360.0f) m_currentRotation -= 360.0f;
        while (m_currentRotation < 0.0f) m_currentRotation += 360.0f;
    }
}

void Boss::UpdateState(float deltaTime) {
    m_stateTimer += deltaTime;
    
    switch (m_currentState) {
        case BossState::IDLE:
            // Idle/standby animation - wait 5 seconds before attacking
            if (m_stateTimer > 5.0f) {
                // Randomly choose an attack
                int attack = GetRandomValue(1, 3);
                if (attack == 1) {
                    SetState(BossState::ATTACK_1);
                    printf("Boss: ATTACK_1\n");
                } else if (attack == 2) {
                    SetState(BossState::ATTACK_2);
                    printf("Boss: ATTACK_2\n");
                } else {
                    SetState(BossState::ATTACK_3);
                    printf("Boss: ATTACK_3\n");
                }
            }
            break;
            
        case BossState::ATTACK_1:
            // Attack 1 logic - lasts 1.5 seconds
            if (m_stateTimer > 1.5f) {
                SetState(BossState::IDLE);
                printf("Boss: Back to IDLE\n");
            }
            break;
            
        case BossState::ATTACK_2:
            // Attack 2 logic - lasts 1.5 seconds
            if (m_stateTimer > 1.5f) {
                SetState(BossState::IDLE);
                printf("Boss: Back to IDLE\n");
            }
            break;
            
        case BossState::ATTACK_3:
            // Attack 3 logic - lasts 1.5 seconds
            if (m_stateTimer > 1.5f) {
                SetState(BossState::IDLE);
                printf("Boss: Back to IDLE\n");
            }
            break;
            
        case BossState::DEATH:
            // Death animation
            break;
    }
}

void Boss::SetState(BossState newState) {
    if (m_currentState != newState) {
        m_currentState = newState;
        m_stateTimer = 0.0f;
        m_currentAnimFrame = 0;
        m_hasAttackedInState = false;  // Reset attack flag when changing state
    }
}

void Boss::Draw() const {
    if (!m_isAlive) return;
    
    // Draw 3D model if loaded
    if (m_modelLoaded) {
        // Get model bounds to calculate proper positioning
        BoundingBox bounds = GetModelBoundingBox(m_model);
        
        // Calculate scale - the hitbox is in game units which are much larger than model units
        // Use a reasonable scale factor instead of trying to match the huge hitbox
        // A scale of ~10-15 should make the boss visible but not enormous
        float uniformScale = 12.0f;  // Fixed scale for now, can adjust based on visual testing
        
        Vector3 modelScale = {uniformScale, uniformScale, uniformScale};
        
        // Adjust position to place model on ground
        // The boss hitbox center is at m_position (200, halfHeight + 5.0f, 0)
        // We want the model's bottom to be above y=0 to account for arena visual thickness
        Vector3 drawPosition = m_position;
        
        // Calculate where the bottom of the scaled model would be relative to its center
        float scaledModelBottom = bounds.min.y * uniformScale;
        
        // The model should be drawn so its bottom is above ground (y=5.0) to clear arena thickness
        // Since the model's center is at origin, we need to lift it by -scaledModelBottom + 5.0
        drawPosition.y = -scaledModelBottom + 5.0f;
        
        // Keep X and Z from m_position for horizontal placement
        drawPosition.x = m_position.x;
        drawPosition.z = m_position.z;
        
        // Draw model with rotation
        DrawModelEx(
            m_model,
            drawPosition,
            {0.0f, 1.0f, 0.0f},  // Rotate around Y axis
            m_currentRotation,
            modelScale,
            WHITE
        );
    } else {
        // Fallback: Draw simple cube if model not loaded
        DrawCube(m_position, m_size.x, m_size.y, m_size.z, m_color);
        DrawCubeWires(m_position, m_size.x, m_size.y, m_size.z, DARKGREEN);
    }
    
    // Draw debug hitbox (toggle with key)
    if (m_showDebugHitbox) {
        AABB hitbox = GetAABB();
        Vector3 hitboxSize = Vector3Subtract(hitbox.max, hitbox.min);
        Vector3 hitboxCenter = hitbox.GetCenter();
        DrawCubeWires(hitboxCenter, hitboxSize.x, hitboxSize.y, hitboxSize.z, YELLOW);
    }
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
    
    m_position.x = Clamp(m_position.x, -ARENA_SIZE + halfWidth, ARENA_SIZE - halfWidth);
    m_position.z = Clamp(m_position.z, -ARENA_SIZE + halfDepth, ARENA_SIZE - halfDepth);
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
