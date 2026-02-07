#pragma once
#include "Entity.hpp"
#include "Config.hpp"
#include "raylib.h"

namespace TimeMaster {

class Player : public Entity, public IDamageable, public ITimedEntity {
private:
    Vector3 m_position;
    float m_radius;
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
    float GetRadius() const { return m_radius; }
    
    // Movement
    void Move(Vector3 direction, float deltaTime);
};

} // namespace TimeMaster
