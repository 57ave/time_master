#pragma once

namespace TimeMaster {

// Screen configuration (fixed)
constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 800;

// Game configuration (fixed)
constexpr int MAX_TOMATOES = 5;
constexpr int MAX_BOSS_PROJECTILES = 10;
constexpr float ARENA_SIZE = 400.0f;
constexpr float ARENA_WALL_THICKNESS = 10.0f;  // Thickness of arena walls for collision
constexpr float ARENA_FLOOR_Y = -50.0f;  // Y position of arena floor where entities stand
constexpr float ARENA_MODEL_Y = -200.0f;  // Y position where arena model is drawn (lower to account for model structure)
constexpr float GRAVITY = 500.0f;  // Gravity acceleration

// Fixed entity sizes
constexpr float PLAYER_RADIUS = 20.0f;
constexpr float BOSS_WIDTH = 60.0f;
constexpr float BOSS_HEIGHT = 80.0f;
constexpr float BOSS_DEPTH = 60.0f;
constexpr float TOMATO_RADIUS = 12.0f;
constexpr float PROJECTILE_RADIUS = 8.0f;

// Fixed camera settings
constexpr float CAMERA_DISTANCE = 180.0f;  // Reduced for better view with smaller entities
constexpr float CAMERA_HEIGHT = 100.0f;    // Reduced proportionally
constexpr float CAMERA_FOV = 45.0f;

// Particle system settings
constexpr int MAX_PARTICLES = 1000;
constexpr float PARTICLE_LIFETIME_MIN = 0.5f;
constexpr float PARTICLE_LIFETIME_MAX = 1.5f;
constexpr float PARTICLE_SIZE_MIN = 2.0f;
constexpr float PARTICLE_SIZE_MAX = 6.0f;
constexpr float PARTICLE_SPEED_MIN = 20.0f;
constexpr float PARTICLE_SPEED_MAX = 80.0f;

// Dash settings
constexpr float DASH_DURATION = 0.5f;         // Duration of dash in seconds
constexpr float DASH_SPEED_BOOST = 400.0f;   // Additional speed during dash (+400 on top of base 80 = 480 total)
constexpr float DASH_COOLDOWN = 0.5f;         // Cooldown between dashes in seconds
constexpr float MAX_STAMINA = 100.0f;         // Maximum stamina
constexpr float STAMINA_REGEN_RATE = 10.0f;   // Stamina regeneration per second (reduced from 25)
constexpr float DASH_STAMINA_COST = 25.0f;    // Stamina cost per dash
constexpr float STAMINA_DEPLETED_TIME_COST = 5.0f; // Time cost when dashing with 0 stamina

/**
 * @brief Singleton class to manage configurable game settings
 */
class GameConfig {
public:
    // Player settings
    float playerSpeed = 80.0f;
    float playerStartingTime = 60.0f;
    float playerMaxTime = 120.0f;
    float playerDamagePerHit = 5.0f;
    
    // Boss settings
    float bossStartingTime = 300.0f;
    float bossDamagePerHit = 3.0f;
    float bossAttackCooldownMin = 1.5f;
    float bossAttackCooldownMax = 2.5f;
    
    // Tomato settings
    float tomatoLifetime = 8.0f;
    float tomatoHealAmount = 5.0f;
    
    // Projectile settings
    float projectileSpeed = 200.0f;
    
    // Camera settings
    float mouseSensitivity = 0.002f;
    
    /**
     * @brief Get the singleton instance
     */
    static GameConfig& GetInstance() {
        static GameConfig instance;
        return instance;
    }
    
    /**
     * @brief Reset all settings to default values
     */
    void ResetToDefaults() {
        playerSpeed = 50.0f;
        playerStartingTime = 60.0f;
        playerMaxTime = 120.0f;
        playerDamagePerHit = 5.0f;
        
        bossStartingTime = 300.0f;
        bossDamagePerHit = 3.0f;
        bossAttackCooldownMin = 1.5f;
        bossAttackCooldownMax = 2.5f;
        
        tomatoLifetime = 8.0f;
        tomatoHealAmount = 5.0f;
        
        projectileSpeed = 200.0f;
        
        mouseSensitivity = 0.002f;
    }
    
private:
    GameConfig() = default;
    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;
};

} // namespace TimeMaster
