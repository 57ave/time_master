#pragma once
#include "GameState.hpp"
#include "Player.hpp"
#include "Boss.hpp"
#include "Tomato.hpp"
#include "Projectile.hpp"
#include "CameraManager.hpp"
#include "HUD.hpp"
#include "Collision.hpp"
#include <vector>
#include <memory>

namespace TimeMaster {

/**
 * @brief Main game class that manages game logic and state
 */
class Game {
private:
    // Game state
    GameState m_state;
    
    // Game entities
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Boss> m_boss;
    std::vector<std::unique_ptr<Tomato>> m_tomatoes;
    std::vector<std::unique_ptr<Projectile>> m_projectiles;       // Boss projectiles
    std::vector<std::unique_ptr<Projectile>> m_playerProjectiles; // Player projectiles
    
    // Systems
    std::unique_ptr<CameraManager> m_cameraManager;
    std::unique_ptr<HUD> m_hud;
    
    // Arena model
    Model m_arenaModel;
    bool m_arenaModelLoaded;
    
    // Spawn timers
    float m_tomatoSpawnTimer;
    float m_playerAttackCooldown;
    
    // Settings menu state
    int m_selectedSetting;
    
public:
    Game();
    ~Game();
    
    /**
     * @brief Initialize/reset the game
     */
    void Init();
    
    /**
     * @brief Update game logic
     */
    void Update();
    
    /**
     * @brief Render the game
     */
    void Draw();
    
    /**
     * @brief Check if game should close
     */
    bool ShouldClose() const;
    
private:
    // State-specific updates
    void UpdateMenu();
    void UpdateSettings();
    void UpdatePlaying();
    void UpdatePaused();
    void UpdateGameOver();
    void UpdateVictory();
    
    // State-specific rendering
    void DrawMenu();
    void DrawSettings();
    void DrawPlaying();
    void DrawPaused();
    void DrawGameOver();
    void DrawVictory();
    
    // Game logic helpers
    void HandlePlayerAttack();
    void HandleBossAttack();
    void UpdateProjectiles(float deltaTime);
    void UpdateTomatoes(float deltaTime);
    void CheckTomatoCollection();
    void SpawnTomato();
    void DrawArena() const;
    
    // State transitions
    void TransitionTo(GameState newState);
};

} // namespace TimeMaster
