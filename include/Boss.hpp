#pragma once
#include "Entity.hpp"
#include "Config.hpp"
#include "Collision.hpp"
#include "BossState.hpp"
#include "raylib.h"

namespace TimeMaster {

// Forward declaration
class Player;

class Boss : public Entity, public IDamageable, public ITimedEntity {
private:
    // Position and physics
    float m_moveSpeed;
    Vector3 m_position;
    Vector3 m_velocity;
    Vector3 m_size;  // Width, Height, Depth (for hitbox)
    
    // Rotation and orientation
    float m_targetRotation;   // Target Y rotation to face player
    float m_currentRotation;  // Current Y rotation (smooth interpolation)
    float m_rotationSpeed;    // Speed of rotation interpolation
    
    // Game state
    float m_time;
    float m_attackCooldown;
    float m_moveTimer;
    bool m_isAlive;
    Color m_color;
    
    // Animation state
    BossState m_currentState;
    float m_stateTimer;       // Time spent in current state
    bool m_hasAttackedInState; // Track if attack was triggered in current attack state
    
    // 3D Model
    Model m_model;
    ModelAnimation* m_animations;
    int m_animationCount;
    int m_currentAnimFrame;
    int m_currentAnimIndex;   // Currently playing animation index
    float m_animTimer;         // Timer for current animation playback
    bool m_modelLoaded;
    
    // Debug
    bool m_showDebugHitbox;
    
    // Private methods
    void UpdateRotation(Vector3 playerPosition, float deltaTime);
    void UpdateState(float deltaTime);
    void LoadModel();
    void UnloadModel();
    void MoveTowards(const Vector3& target, float deltaTime);
    
public:
    Boss();
    ~Boss();
    
    // Entity interface
    void Update(float deltaTime) override;
    void UpdateWithPlayer(Vector3 playerPosition, float deltaTime);
    void Draw() const override;
    bool IsActive() const override { return m_isAlive; }
    Vector3 GetPosition() const override { return m_position; }
    
    // IDamageable interface
    void TakeDamage(float damage) override;
    bool IsAlive() const override { return m_isAlive; }
    float GetHealth() const override { return m_time; }
    
    // ITimedEntity interface
    std::string GetTimeString() const override;
    float GetTime() const override { return m_time; }
    
    // Boss specific methods
    void Reset();
    bool CanAttack() const;
    void ResetAttackCooldown();
    bool CheckCollisionWithPlayer(const Player& player) const;
    void ToggleDebugHitbox() { m_showDebugHitbox = !m_showDebugHitbox; }
    
    // State management
    void SetState(BossState newState);
    BossState GetState() const { return m_currentState; }
    bool ShouldTriggerAttack() const { return !m_hasAttackedInState; }
    void MarkAttackTriggered() { m_hasAttackedInState = true; }
    
    // Collision
    AABB GetAABB() const;
    void ApplyPushback(Vector3 pushback);
    Vector3 GetSize() const { return m_size; }
    void SetPosition(Vector3 position) { m_position = position; }
};

} // namespace TimeMaster
