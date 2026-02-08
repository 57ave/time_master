#pragma once
#include "raylib.h"
#include "raymath.h"

namespace TimeMaster {

/**
 * @brief Single particle in the particle system
 */
struct Particle {
    Vector3 position;
    Vector3 velocity;
    Color color;
    float lifetime;      // Total lifetime in seconds
    float age;          // Current age in seconds
    float size;         // Particle size
    bool active;
    
    Particle() 
        : position({0, 0, 0})
        , velocity({0, 0, 0})
        , color(WHITE)
        , lifetime(1.0f)
        , age(0.0f)
        , size(1.0f)
        , active(false) {}
    
    void Update(float deltaTime) {
        if (!active) return;
        
        age += deltaTime;
        if (age >= lifetime) {
            active = false;
            return;
        }
        
        // Update position based on velocity
        position = Vector3Add(position, Vector3Scale(velocity, deltaTime));
        
        // Apply gravity (optional)
        velocity.y -= 200.0f * deltaTime;
        
        // Fade out alpha based on age
        float lifeRatio = age / lifetime;
        color.a = (unsigned char)((1.0f - lifeRatio) * 255);
    }
    
    void Draw() const {
        if (!active) return;
        
        // Draw as a sphere for better visibility from all angles
        DrawSphere(position, size, color);
        // Also draw a wireframe for extra visibility
        DrawSphereWires(position, size * 1.1f, 4, 4, Fade(color, 0.5f));
    }
};

/**
 * @brief Type of particle emitter
 */
enum class ParticleEmitterType {
    FOOTSTEP,        // Emitted when player/boss walks
    DASH,            // Emitted when player dashes
    BOSS_AOE,        // Emitted during boss AoE attacks
    BOSS_PROJECTILE, // Trail for boss projectiles
    IMPACT,          // Impact effect when projectile hits
    PLAYER_SHOOT,    // Emitted when player shoots
    MELEE_ATTACK     // Emitted when player does melee attack
};

/**
 * @brief Particle system/emitter
 */
class ParticleSystem {
private:
    static constexpr int MAX_PARTICLES = 200;
    Particle m_particles[MAX_PARTICLES];
    ParticleEmitterType m_type;
    
public:
    ParticleSystem(ParticleEmitterType type = ParticleEmitterType::FOOTSTEP);
    
    /**
     * @brief Emit particles at a position
     * @param position Origin point for particles
     * @param count Number of particles to emit
     * @param color Base color for particles
     */
    void Emit(Vector3 position, int count, Color color);
    
    /**
     * @brief Emit particles for footstep effect
     */
    void EmitFootstep(Vector3 position);
    
    /**
     * @brief Emit particles for dash effect
     * @param position Origin point for dash particles
     * @param direction Direction of the dash
     */
    void EmitDash(Vector3 position, Vector3 direction);
    
    /**
     * @brief Emit particles for boss AoE attack
     * @param center Center of the AoE
     * @param radius Radius of the AoE
     */
    void EmitBossAoE(Vector3 center, float radius);
    
    /**
     * @brief Emit particles when player shoots
     * @param position Shooting position
     * @param direction Shooting direction
     */
    void EmitPlayerShoot(Vector3 position, Vector3 direction);
    
    /**
     * @brief Emit particles for melee attack
     * @param position Attack position
     * @param direction Attack direction
     */
    void EmitMeleeAttack(Vector3 position, Vector3 direction);
    
    /**
     * @brief Update all active particles
     */
    void Update(float deltaTime);
    
    /**
     * @brief Draw all active particles
     */
    void Draw() const;
    
    /**
     * @brief Clear all particles
     */
    void Clear();
    
private:
    Particle* GetInactiveParticle();
};

} // namespace TimeMaster
