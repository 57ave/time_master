#pragma once
#include "Entity.hpp"
#include "Config.hpp"
#include "Player.hpp"
#include "raylib.h"

namespace TimeMaster {

class Boss : public Entity, public IDamageable, public ITimedEntity {
private:
    Vector3 m_position;
    Vector3 m_velocity;
    float m_width;
    float m_height;
    float m_depth;
    float m_time;
    float m_attackCooldown;
    float m_moveTimer;
    bool m_isAlive;
    Color m_color;
    
public:
    Boss();
    
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
    
    // Boss specific methods
    void Reset();
    bool CanAttack() const;
    void ResetAttackCooldown();
    bool CheckCollisionWithPlayer(const Player& player) const;
    BoundingBox GetBoundingBox() const;
};

} // namespace TimeMaster
