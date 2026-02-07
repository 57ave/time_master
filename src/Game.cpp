#include "Game.hpp"
#include "BossState.hpp"
#include "raymath.h"
#include <cstdlib>
#include <ctime>

namespace TimeMaster {

Game::Game() 
    : m_state(GameState::MENU)
    , m_tomatoSpawnTimer(0.0f)
    , m_playerAttackCooldown(0.0f)
    , m_selectedSetting(0) {
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Load static assets
    Tomato::LoadModel();
    
    // Initialize systems
    m_cameraManager = std::make_unique<CameraManager>();
    m_hud = std::make_unique<HUD>();
    
    // Initialize entities
    m_player = std::make_unique<Player>();
    m_boss = std::make_unique<Boss>();
    
    // Initialize tomato pool
    m_tomatoes.reserve(MAX_TOMATOES);
    for (int i = 0; i < MAX_TOMATOES; ++i) {
        m_tomatoes.push_back(std::make_unique<Tomato>());
    }
    
    // Initialize projectile pool
    m_projectiles.reserve(MAX_BOSS_PROJECTILES);
    for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i) {
        m_projectiles.push_back(std::make_unique<Projectile>());
    }
    
    // Initialize player projectile pool
    m_playerProjectiles.reserve(MAX_BOSS_PROJECTILES);
    for (int i = 0; i < MAX_BOSS_PROJECTILES; ++i) {
        m_playerProjectiles.push_back(std::make_unique<Projectile>());
    }
}

Game::~Game() {
    // Unload static assets
    Tomato::UnloadModel();
}

void Game::Init() {
    m_player->Reset();
    m_boss->Reset();
    m_cameraManager->Reset();
    m_tomatoSpawnTimer = 0.0f;
    m_playerAttackCooldown = 0.0f;
    
    // Ensure cursor is locked for gameplay
    DisableCursor();
    
    // Clear any accumulated mouse delta from menu
    GetMouseDelta();
    
    // Reset all tomatoes and projectiles
    for (auto& tomato : m_tomatoes) {
        tomato->OnCollect();
    }
    for (auto& projectile : m_projectiles) {
        projectile->Deactivate();
    }
}

void Game::Update() {
    switch (m_state) {
        case GameState::MENU:
            UpdateMenu();
            break;
        case GameState::SETTINGS:
            UpdateSettings();
            break;
        case GameState::PLAYING:
            UpdatePlaying();
            break;
        case GameState::PAUSED:
            UpdatePaused();
            break;
        case GameState::GAME_OVER:
            UpdateGameOver();
            break;
        case GameState::VICTORY:
            UpdateVictory();
            break;
    }
}

void Game::Draw() {
    switch (m_state) {
        case GameState::MENU:
            DrawMenu();
            break;
        case GameState::SETTINGS:
            DrawSettings();
            break;
        case GameState::PLAYING:
            DrawPlaying();
            break;
        case GameState::PAUSED:
            DrawPaused();
            break;
        case GameState::GAME_OVER:
            DrawGameOver();
            break;
        case GameState::VICTORY:
            DrawVictory();
            break;
    }
}

bool Game::ShouldClose() const {
    return WindowShouldClose();
}

void Game::UpdateMenu() {
    if (IsKeyPressed(KEY_ENTER)) {
        Init();
        TransitionTo(GameState::PLAYING);
    }
    if (IsKeyPressed(KEY_S)) {
        m_selectedSetting = 0;
        TransitionTo(GameState::SETTINGS);
    }
}

void Game::UpdateSettings() {
    auto& config = GameConfig::GetInstance();
    
    // Navigate settings
    if (IsKeyPressed(KEY_UP)) {
        m_selectedSetting--;
        if (m_selectedSetting < 0) m_selectedSetting = 7; // 8 options (0-7)
    }
    if (IsKeyPressed(KEY_DOWN)) {
        m_selectedSetting++;
        if (m_selectedSetting > 7) m_selectedSetting = 0;
    }
    
    // Adjust values
    float adjustSpeed = 1.0f;
    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
        adjustSpeed = 5.0f; // Fast adjustment
    }
    
    if (IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT)) {
        switch (m_selectedSetting) {
            case 0: // Mouse Sensitivity
                config.mouseSensitivity -= 0.01f;
                if (config.mouseSensitivity < 0.01f) config.mouseSensitivity = 0.01f;
                m_cameraManager->SetMouseSensitivity(config.mouseSensitivity);
                break;
            case 1: // Player Speed
                config.playerSpeed -= 10.0f * adjustSpeed;
                if (config.playerSpeed < 50.0f) config.playerSpeed = 50.0f;
                break;
            case 2: // Player Starting Time
                config.playerStartingTime -= 5.0f * adjustSpeed;
                if (config.playerStartingTime < 10.0f) config.playerStartingTime = 10.0f;
                break;
            case 3: // Player Max Time
                config.playerMaxTime -= 10.0f * adjustSpeed;
                if (config.playerMaxTime < 60.0f) config.playerMaxTime = 60.0f;
                break;
            case 4: // Player Damage
                config.playerDamagePerHit -= 0.5f * adjustSpeed;
                if (config.playerDamagePerHit < 1.0f) config.playerDamagePerHit = 1.0f;
                break;
            case 5: // Boss Starting Time
                config.bossStartingTime -= 10.0f * adjustSpeed;
                if (config.bossStartingTime < 60.0f) config.bossStartingTime = 60.0f;
                break;
            case 6: // Boss Damage
                config.bossDamagePerHit -= 0.5f * adjustSpeed;
                if (config.bossDamagePerHit < 0.5f) config.bossDamagePerHit = 0.5f;
                break;
            case 7: // Tomato Heal
                config.tomatoHealAmount -= 0.5f * adjustSpeed;
                if (config.tomatoHealAmount < 1.0f) config.tomatoHealAmount = 1.0f;
                break;
        }
    }
    
    if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)) {
        switch (m_selectedSetting) {
            case 0: // Mouse Sensitivity
                config.mouseSensitivity += 0.01f;
                if (config.mouseSensitivity > 1.0f) config.mouseSensitivity = 1.0f;
                m_cameraManager->SetMouseSensitivity(config.mouseSensitivity);
                break;
            case 1: // Player Speed
                config.playerSpeed += 10.0f * adjustSpeed;
                if (config.playerSpeed > 500.0f) config.playerSpeed = 500.0f;
                break;
            case 2: // Player Starting Time
                config.playerStartingTime += 5.0f * adjustSpeed;
                if (config.playerStartingTime > 300.0f) config.playerStartingTime = 300.0f;
                break;
            case 3: // Player Max Time
                config.playerMaxTime += 10.0f * adjustSpeed;
                if (config.playerMaxTime > 600.0f) config.playerMaxTime = 600.0f;
                break;
            case 4: // Player Damage
                config.playerDamagePerHit += 0.5f * adjustSpeed;
                if (config.playerDamagePerHit > 50.0f) config.playerDamagePerHit = 50.0f;
                break;
            case 5: // Boss Starting Time
                config.bossStartingTime += 10.0f * adjustSpeed;
                if (config.bossStartingTime > 1000.0f) config.bossStartingTime = 1000.0f;
                break;
            case 6: // Boss Damage
                config.bossDamagePerHit += 0.5f * adjustSpeed;
                if (config.bossDamagePerHit > 50.0f) config.bossDamagePerHit = 50.0f;
                break;
            case 7: // Tomato Heal
                config.tomatoHealAmount += 0.5f * adjustSpeed;
                if (config.tomatoHealAmount > 50.0f) config.tomatoHealAmount = 50.0f;
                break;
        }
    }
    
    // Reset to defaults
    if (IsKeyPressed(KEY_R)) {
        config.ResetToDefaults();
        m_cameraManager->SetMouseSensitivity(config.mouseSensitivity);
    }
    
    // Return to menu
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
        TransitionTo(GameState::MENU);
    }
}

void Game::UpdatePlaying() {
    float deltaTime = GetFrameTime();
    
    // Decrease time for player and boss automatically
    m_player->TakeDamage(deltaTime);
    m_boss->TakeDamage(deltaTime);
    
    // Update camera to follow player with mouse control
    m_cameraManager->UpdateThirdPerson(m_player->GetPosition(), deltaTime);
    
    // Update player with camera-relative movement
    Vector3 cameraForward = m_cameraManager->GetForwardDirection();
    Vector3 cameraRight = m_cameraManager->GetRightDirection();
    m_player->UpdateWithCamera(deltaTime, cameraForward, cameraRight);
    
    // Update boss with player position for smooth rotation
    m_boss->UpdateWithPlayer(m_player->GetPosition(), deltaTime);
    
    // Update player attack cooldown
    m_playerAttackCooldown -= deltaTime;
    if (m_playerAttackCooldown < 0) m_playerAttackCooldown = 0;
    
    // Toggle camera mode with C key
    if (IsKeyPressed(KEY_C)) {
        m_cameraManager->ToggleMode();
    }
    
<<<<<<< HEAD
    // Handle player melee attack
=======
    // Toggle cursor lock with ESC key (for debugging or menu access)
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_cameraManager->ToggleCursorLock();
    }
    
    // Toggle boss debug hitbox with H key
    if (IsKeyPressed(KEY_H)) {
        m_boss->ToggleDebugHitbox();
    }
    
    // Handle player attack
>>>>>>> 32930cc (add animation boss)
    if (IsKeyPressed(KEY_SPACE)) {
        HandlePlayerAttack();
    }
    
    // Handle player projectile attack (Left Mouse Button)
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && m_playerAttackCooldown <= 0) {
        // Shoot projectile toward boss
        for (auto& projectile : m_playerProjectiles) {
            if (!projectile->IsActive()) {
                projectile->Launch(m_player->GetPosition(), m_boss->GetPosition());
                m_playerAttackCooldown = 0.5f; // Half second cooldown
                break;
            }
        }
    }
    
    // Resolve collision between player and boss (prevent overlap)
    if (m_player->IsAlive() && m_boss->IsAlive()) {
        AABB playerAABB = m_player->GetAABB();
        AABB bossAABB = m_boss->GetAABB();
        
        CollisionResolution collision = ResolveAABBCollision(playerAABB, bossAABB);
        if (collision.hasCollision) {
            // Push the player away from the boss
            // We push only the player to keep the boss movement stable
            m_player->ApplyPushback(collision.pushback);
        }
    }
    
    // Handle boss attack based on state machine
    BossState bossState = m_boss->GetState();
    if ((bossState == BossState::ATTACK_1 || 
         bossState == BossState::ATTACK_2 || 
         bossState == BossState::ATTACK_3) && 
        m_boss->ShouldTriggerAttack()) {
        HandleBossAttack();
        m_boss->MarkAttackTriggered();
    }
    
    // Update projectiles
    UpdateProjectiles(deltaTime);
    
    // Update player projectiles
    auto& config = GameConfig::GetInstance();
    for (auto& projectile : m_playerProjectiles) {
        projectile->Update(deltaTime);
        
        if (projectile->CheckCollision(m_boss->GetPosition(), m_boss->GetSize().x / 2.0f)) {
            m_boss->TakeDamage(config.playerDamagePerHit);
            projectile->Deactivate();
        }
    }
    
    // Update tomatoes
    UpdateTomatoes(deltaTime);
    
    // Spawn tomatoes
    m_tomatoSpawnTimer += deltaTime;
    if (m_tomatoSpawnTimer > 3.0f) { // Spawn every 3 seconds on average
        if (GetRandomValue(0, 1) == 1) {
            SpawnTomato();
        }
        m_tomatoSpawnTimer = 0.0f;
    }
    
    // Check tomato collection
    CheckTomatoCollection();
    
    // Check game over conditions
    if (!m_player->IsAlive()) {
        TransitionTo(GameState::GAME_OVER);
    }
    if (!m_boss->IsAlive()) {
        TransitionTo(GameState::VICTORY);
    }
    
    // Pause
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
        TransitionTo(GameState::PAUSED);
    }
}

void Game::UpdatePaused() {
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ENTER)) {
        DisableCursor(); // Re-lock cursor when resuming
        GetMouseDelta(); // Clear accumulated mouse delta
        TransitionTo(GameState::PLAYING);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        TransitionTo(GameState::MENU);
    }
}

void Game::UpdateGameOver() {
    if (IsKeyPressed(KEY_ENTER)) {
        Init();
        TransitionTo(GameState::PLAYING);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        TransitionTo(GameState::MENU);
    }
}

void Game::UpdateVictory() {
    if (IsKeyPressed(KEY_ENTER)) {
        Init();
        TransitionTo(GameState::PLAYING);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        TransitionTo(GameState::MENU);
    }
}

void Game::DrawMenu() {
    m_hud->DrawMenu();
}

void Game::DrawSettings() {
    m_hud->DrawSettings(m_selectedSetting);
}

void Game::DrawPlaying() {
    BeginMode3D(m_cameraManager->GetCamera());
    
    DrawArena();
    
    // Draw all entities
    m_player->Draw();
    m_boss->Draw();
    
    for (const auto& tomato : m_tomatoes) {
        tomato->Draw();
    }
    
    for (const auto& projectile : m_projectiles) {
        projectile->Draw();
    }
    
    for (const auto& projectile : m_playerProjectiles) {
        projectile->Draw();
    }
    
    EndMode3D();
    
    // Draw HUD
    m_hud->Draw(*m_player, *m_boss);
    
    // Draw attack hint
    float distance = Vector3Distance(m_player->GetPosition(), m_boss->GetPosition());
    m_hud->DrawAttackHint(distance);
}

void Game::DrawPaused() {
    // Draw game in background
    DrawPlaying();
    
    // Draw pause overlay
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.5f));
    DrawText("PAUSED", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 50, WHITE);
    DrawText("Press P or ENTER to Continue", SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 + 20, 20, WHITE);
    DrawText("Press ESC for Menu", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 50, 20, WHITE);
}

void Game::DrawGameOver() {
    m_hud->DrawGameOver();
}

void Game::DrawVictory() {
    m_hud->DrawVictory();
}

void Game::HandlePlayerAttack() {
    auto& config = GameConfig::GetInstance();
    if (m_boss->CheckCollisionWithPlayer(*m_player)) {
        m_boss->TakeDamage(config.bossDamagePerHit);
    }
}

void Game::HandleBossAttack() {
    for (auto& projectile : m_projectiles) {
        if (!projectile->IsActive()) {
            projectile->Launch(m_boss->GetPosition(), m_player->GetPosition());
            break;
        }
    }
}

void Game::UpdateProjectiles(float deltaTime) {
    auto& config = GameConfig::GetInstance();
    for (auto& projectile : m_projectiles) {
        projectile->Update(deltaTime);
        
        if (projectile->CheckCollision(m_player->GetPosition(), m_player->GetApproxRadius())) {
            m_player->TakeDamage(config.playerDamagePerHit);
        }
    }
}

void Game::UpdateTomatoes(float deltaTime) {
    for (auto& tomato : m_tomatoes) {
        tomato->Update(deltaTime);
    }
}

void Game::CheckTomatoCollection() {
    auto& config = GameConfig::GetInstance();
    for (auto& tomato : m_tomatoes) {
        if (tomato->CheckCollision(m_player->GetPosition(), m_player->GetApproxRadius())) {
            m_player->Heal(config.tomatoHealAmount);
            tomato->OnCollect();
        }
    }
}

void Game::SpawnTomato() {
    for (auto& tomato : m_tomatoes) {
        if (!tomato->IsActive()) {
            float x = static_cast<float>(GetRandomValue(-ARENA_SIZE + 50, ARENA_SIZE - 50));
            float z = static_cast<float>(GetRandomValue(-ARENA_SIZE + 50, ARENA_SIZE - 50));
            tomato->Spawn(x, 15.0f, z);
            break;
        }
    }
}

void Game::DrawArena() const {
    // Draw floor
    DrawPlane({0.0f, 0.0f, 0.0f}, {ARENA_SIZE * 2, ARENA_SIZE * 2}, LIGHTGRAY);
    DrawGrid(40, 50.0f);
    
    // Draw arena walls
    DrawCubeWires({0, 50, -ARENA_SIZE}, ARENA_SIZE * 2, 100, 2, DARKGRAY);
    DrawCubeWires({0, 50, ARENA_SIZE}, ARENA_SIZE * 2, 100, 2, DARKGRAY);
    DrawCubeWires({-ARENA_SIZE, 50, 0}, 2, 100, ARENA_SIZE * 2, DARKGRAY);
    DrawCubeWires({ARENA_SIZE, 50, 0}, 2, 100, ARENA_SIZE * 2, DARKGRAY);
}

void Game::TransitionTo(GameState newState) {
    m_state = newState;
    
    // Handle cursor state based on game state
    switch (newState) {
        case GameState::MENU:
        case GameState::SETTINGS:
        case GameState::PAUSED:
        case GameState::GAME_OVER:
        case GameState::VICTORY:
            EnableCursor();
            break;
        case GameState::PLAYING:
            DisableCursor();
            break;
    }
}

} // namespace TimeMaster
