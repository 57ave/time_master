#pragma once
#include "Entity.hpp"
#include "Config.hpp"
#include "raylib.h"

namespace TimeMaster {

class Projectile : public Entity {
private:
    Vector3 m_position;
    Vector3 m_velocity;
    float m_radius;
    bool m_active;
    Color m_color;
    
public:
    Projectile();
    
    // Entity interface
    void Update(float deltaTime) override;
    void Draw() const override;
    bool IsActive() const override { return m_active; }
    Vector3 GetPosition() const override { return m_position; }
    
    // Projectile specific methods
    void Launch(Vector3 startPos, Vector3 targetPos);
    void Deactivate();
    bool CheckCollision(Vector3 pos, float otherRadius);
    float GetRadius() const { return m_radius; }
};

} // namespace TimeMaster
