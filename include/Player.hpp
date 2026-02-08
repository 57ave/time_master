#pragma once
#include "Entity.hpp"
#include "Config.hpp"
#include "Collision.hpp"
#include "raylib.h"

namespace TimeMaster {

class Player : public Entity, public IDamageable, public ITimedEntity {
private:
    Vector3 m_position;
    Vector3 m_velocity;     // Velocity for physics (gravity)
    Vector3 m_size;         // Width, Height, Depth of hitbox
    float m_speed;
    float m_time;
    bool m_isAlive;
    Color m_color;
    
    // Animation state
    int m_currentAnimFrame;
    int m_currentAnimIndex;
    float m_animTimer;
    bool m_isMoving;
    bool m_isRunning;       // Running state (shift key)
    float m_rotationAngle;  // Rotation angle to face camera
    
    // Dash state
    bool m_isDashing;
    float m_dashTimer;
    float m_dashCooldownTimer;
    Vector3 m_dashDirection;
    float m_stamina;  // Stamina for dashing
    
    // Static model (shared by all players, though typically only one exists)
    static Model s_model;
    static bool s_modelLoaded;
    static ModelAnimation* s_animations;
    static int s_animationCount;
    
public:
    Player();
    ~Player();
    
    /**
     * @brief Load shared player model (call once)
     */
    static void LoadModel();
    
    /**
     * @brief Unload shared player model (call once on cleanup)
     */
    static void UnloadModel();
    
    // Entity interface
    void Update(float deltaTime) override;
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
    
    // Player specific methods
    void Heal(float amount);
    void Reset();
    
    // Collision
    AABB GetAABB() const;
    void ApplyPushback(Vector3 pushback);
    Vector3 GetSize() const { return m_size; }
    
    // Approximate radius for sphere-based collision (backward compatibility)
    float GetApproxRadius() const { 
        // Use average of width and depth as radius approximation
        return (m_size.x + m_size.z) / 4.0f; 
    }
    
    // Movement
    void Move(Vector3 direction, float deltaTime);
    void SetPosition(Vector3 position) { m_position = position; }
    void SetCameraAngle(float angle) { m_rotationAngle = angle; }
    void UpdateWithCamera(float deltaTime, Vector3 cameraForward, Vector3 cameraRight);
<<<<<<< Updated upstream

    void ClampToArenaCircle();
=======
    
    // Dash
    void StartDash(Vector3 direction);
    bool IsDashing() const { return m_isDashing; }
    bool CanDash() const { return !m_isDashing && m_dashCooldownTimer <= 0.0f; }
    float GetStamina() const { return m_stamina; }
>>>>>>> Stashed changes
};

} // namespace TimeMaster
