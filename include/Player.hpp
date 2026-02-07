#pragma once
#include "Entity.hpp"
#include "Config.hpp"
#include "Collision.hpp"
#include "raylib.h"

namespace TimeMaster {

class Player : public Entity, public IDamageable, public ITimedEntity {
private:
    Vector3 m_position;
    Vector3 m_size;         // Width, Height, Depth of hitbox
    float m_speed;
    float m_time;
    bool m_isAlive;
    Color m_color;
    
public:
    Player();
    
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
    void UpdateWithCamera(float deltaTime, Vector3 cameraForward, Vector3 cameraRight);
};

} // namespace TimeMaster
