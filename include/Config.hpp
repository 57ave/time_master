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
constexpr float CAMERA_DISTANCE = 200.0f;  // Reduced for better view with smaller entities
constexpr float CAMERA_HEIGHT = 150.0f;    // Reduced proportionally
constexpr float CAMERA_FOV = 45.0f;

/**
 * @brief Singleton class to manage configurable game settings
 */
class GameConfig {
public:
    // Player settings
    float playerSpeed = 150.0f;
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
        playerSpeed = 150.0f;
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
