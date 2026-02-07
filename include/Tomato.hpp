#pragma once
#include "Entity.hpp"
#include "Config.hpp"
#include "raylib.h"

namespace TimeMaster {

class Tomato : public Entity, public ICollectible {
private:
    Vector3 m_position;
    float m_radius;
    float m_lifetime;
    float m_rotationAngle;
    bool m_active;
    
public:
    Tomato();
    
    // Entity interface
    void Update(float deltaTime) override;
    void Draw() const override;
    bool IsActive() const override { return m_active; }
    Vector3 GetPosition() const override { return m_position; }
    
    // ICollectible interface
    void OnCollect() override;
    float GetCollectionRadius() const override { return m_radius; }
    
    // Tomato specific methods
    void Spawn(float x, float y, float z);
    bool CheckCollision(Vector3 pos, float otherRadius) const;
};

} // namespace TimeMaster
