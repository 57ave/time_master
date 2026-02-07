#pragma once
#include "raylib.h"
#include <string>

namespace TimeMaster {

/**
 * @brief Abstract base class for all game entities
 */
class Entity {
public:
    virtual ~Entity() = default;
    
    /**
     * @brief Update entity state
     * @param deltaTime Time elapsed since last frame
     */
    virtual void Update(float deltaTime) = 0;
    
    /**
     * @brief Render the entity
     */
    virtual void Draw() const = 0;
    
    /**
     * @brief Check if entity is currently active
     */
    virtual bool IsActive() const = 0;
    
    /**
     * @brief Get entity position
     */
    virtual Vector3 GetPosition() const = 0;
};

/**
 * @brief Interface for entities that can be collected
 */
class ICollectible {
public:
    virtual ~ICollectible() = default;
    
    /**
     * @brief Called when entity is collected
     */
    virtual void OnCollect() = 0;
    
    /**
     * @brief Get collection radius
     */
    virtual float GetCollectionRadius() const = 0;
};

/**
 * @brief Interface for entities that can deal damage
 */
class IDamageable {
public:
    virtual ~IDamageable() = default;
    
    /**
     * @brief Apply damage to entity
     * @param damage Amount of damage to apply
     */
    virtual void TakeDamage(float damage) = 0;
    
    /**
     * @brief Check if entity is still alive
     */
    virtual bool IsAlive() const = 0;
    
    /**
     * @brief Get current health/time
     */
    virtual float GetHealth() const = 0;
};

/**
 * @brief Interface for time-based UI display
 */
class ITimedEntity {
public:
    virtual ~ITimedEntity() = default;
    
    /**
     * @brief Get time as formatted string (MM:SS)
     */
    virtual std::string GetTimeString() const = 0;
    
    /**
     * @brief Get time value in seconds
     */
    virtual float GetTime() const = 0;
};

} // namespace TimeMaster
