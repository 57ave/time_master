#include "Particle.hpp"
#include <cstdlib>

namespace TimeMaster {

ParticleSystem::ParticleSystem(ParticleEmitterType type)
    : m_type(type) {
    // Initialize all particles as inactive
    for (int i = 0; i < MAX_PARTICLES; i++) {
        m_particles[i].active = false;
    }
}

Particle* ParticleSystem::GetInactiveParticle() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!m_particles[i].active) {
            return &m_particles[i];
        }
    }
    return nullptr;
}

void ParticleSystem::Emit(Vector3 position, int count, Color color) {
    for (int i = 0; i < count; i++) {
        Particle* p = GetInactiveParticle();
        if (!p) break;
        
        p->position = position;
        p->color = color;
        p->lifetime = 0.5f + (float)(rand() % 100) / 200.0f; // 0.5 to 1.0 seconds
        p->age = 0.0f;
        p->size = 0.5f + (float)(rand() % 100) / 200.0f; // 0.5 to 1.0 units
        p->active = true;
        
        // Random velocity in horizontal directions
        float angle = (float)(rand() % 360) * DEG2RAD;
        float speed = 10.0f + (float)(rand() % 100) / 10.0f; // 10 to 20 units/s
        
        p->velocity.x = cosf(angle) * speed;
        p->velocity.y = 20.0f + (float)(rand() % 100) / 10.0f; // Upward velocity
        p->velocity.z = sinf(angle) * speed;
    }
}

void ParticleSystem::EmitFootstep(Vector3 position) {
    // Emit small dust particles at ground level
    position.y += 0.5f; // Slightly above ground
    
    Color dustColor = {139, 90, 43, 255}; // Brown dust
    Emit(position, 3, dustColor); // Few particles for footsteps
}

void ParticleSystem::EmitDash(Vector3 position, Vector3 direction) {
    // Emit trail particles behind the dash
    int particleCount = 15;
    
    for (int i = 0; i < particleCount; i++) {
        Particle* p = GetInactiveParticle();
        if (!p) break;
        
        // Position particles slightly behind and around the player
        Vector3 offset = {
            (float)(rand() % 100) / 50.0f - 1.0f, // -1 to 1
            (float)(rand() % 100) / 50.0f - 1.0f,
            (float)(rand() % 100) / 50.0f - 1.0f
        };
        
        p->position = Vector3Add(position, offset);
        
        // Cyan/blue color for dash effect
        p->color = {0, 200, 255, 255};
        p->lifetime = 0.3f; // Short-lived for speed effect
        p->age = 0.0f;
        p->size = 1.5f + (float)(rand() % 100) / 100.0f;
        p->active = true;
        
        // Velocity opposite to dash direction (trail effect)
        p->velocity.x = -direction.x * 50.0f + (float)(rand() % 100) / 5.0f - 10.0f;
        p->velocity.y = 10.0f + (float)(rand() % 100) / 10.0f;
        p->velocity.z = -direction.z * 50.0f + (float)(rand() % 100) / 5.0f - 10.0f;
    }
}

void ParticleSystem::EmitBossAoE(Vector3 center, float radius) {
    // Emit particles in a circle around the AoE
    int particleCount = 30; // More particles for dramatic effect
    
    for (int i = 0; i < particleCount; i++) {
        Particle* p = GetInactiveParticle();
        if (!p) break;
        
        // Position particles around the radius
        float angle = (float)i / particleCount * 2.0f * PI;
        float distance = radius * (0.8f + (float)(rand() % 40) / 100.0f); // 80-120% of radius
        
        p->position.x = center.x + cosf(angle) * distance;
        p->position.y = center.y + 1.0f;
        p->position.z = center.z + sinf(angle) * distance;
        
        p->color = {255, 50, 50, 255}; // Red for danger
        p->lifetime = 0.8f;
        p->age = 0.0f;
        p->size = 2.0f;
        p->active = true;
        
        // Velocity outward from center
        p->velocity.x = cosf(angle) * 30.0f;
        p->velocity.y = 40.0f + (float)(rand() % 100) / 5.0f;
        p->velocity.z = sinf(angle) * 30.0f;
    }
}

void ParticleSystem::EmitPlayerShoot(Vector3 position, Vector3 direction) {
    // Emit muzzle flash / shooting particles
    int particleCount = 20; // Increased for better visibility
    
    TraceLog(LOG_INFO, "EmitPlayerShoot at (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    
    for (int i = 0; i < particleCount; i++) {
        Particle* p = GetInactiveParticle();
        if (!p) break;
        
        // Position at shooting point with slight spread, elevated to player's chest height
        p->position = position;
        p->position.y += 5.0f; // Raise to player's upper body
        
        // Bright orange/yellow color for shooting effect
        p->color = {255, 200, 0, 255};
        p->lifetime = 0.5f; // Longer lifetime for better visibility
        p->age = 0.0f;
        p->size = 2.0f + (float)(rand() % 100) / 50.0f; // Larger particles
        p->active = true;
        
        // Velocity in shooting direction with spread
        float spread = 40.0f;
        p->velocity.x = direction.x * 80.0f + (float)(rand() % 100) / 50.0f * spread - spread/2;
        p->velocity.y = direction.y * 80.0f + (float)(rand() % 100) / 50.0f * spread;
        p->velocity.z = direction.z * 80.0f + (float)(rand() % 100) / 50.0f * spread - spread/2;
    }
}

void ParticleSystem::EmitMeleeAttack(Vector3 position, Vector3 direction) {
    // Emit slash/swing particles
    int particleCount = 25; // Increased for better visibility
    
    TraceLog(LOG_INFO, "EmitMeleeAttack at (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    
    for (int i = 0; i < particleCount; i++) {
        Particle* p = GetInactiveParticle();
        if (!p) break;
        
        // Arc particles in front of player in a wider sweep
        float arcAngle = ((float)i / particleCount - 0.5f) * PI * 1.2f; // Wider arc
        
        // Position particles in front of player at chest height
        p->position.x = position.x + direction.x * 3.0f + cosf(arcAngle) * 4.0f;
        p->position.y = position.y + 5.0f + (float)(rand() % 100) / 50.0f; // At player's upper body
        p->position.z = position.z + direction.z * 3.0f + sinf(arcAngle) * 4.0f;
        
        // Bright cyan/white color for melee slash
        p->color = {150, 200, 255, 255};
        p->lifetime = 0.5f; // Longer lifetime
        p->age = 0.0f;
        p->size = 2.5f; // Larger particles
        p->active = true;
        
        // Velocity outward in arc
        p->velocity.x = direction.x * 40.0f + cosf(arcAngle) * 50.0f;
        p->velocity.y = 15.0f;
        p->velocity.z = direction.z * 40.0f + sinf(arcAngle) * 50.0f;
    }
}

void ParticleSystem::Update(float deltaTime) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (m_particles[i].active) {
            m_particles[i].Update(deltaTime);
        }
    }
}

void ParticleSystem::Draw() const {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (m_particles[i].active) {
            m_particles[i].Draw();
        }
    }
}

void ParticleSystem::Clear() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        m_particles[i].active = false;
    }
}

} // namespace TimeMaster
