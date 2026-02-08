#include "Game.hpp"
#include "BossState.hpp"
#include "raymath.h"
#include <cstdlib>
#include <ctime>

namespace TimeMaster {

Game::Game() 
    : m_state(GameState::MENU)
    , m_arenaModel{0}
    , m_arenaModelLoaded(false)
    , m_tomatoSpawnTimer(0.0f)
    , m_playerAttackCooldown(0.0f)
    , m_lastPlayerPosition({0, 0, 0})
    , m_lastBossPosition({0, 0, 0})
    , m_playerFootstepTimer(0.0f)
    , m_bossFootstepTimer(0.0f)
    , m_dashParticleTimer(0.0f)
    , m_selectedSetting(0) {
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Load static assets
    Player::LoadModel();
    Tomato::LoadModel();
    
    // Load arena model
    m_arenaModel = LoadModel("assets/models/arena/scene.gltf");
    if (m_arenaModel.meshCount > 0 && m_arenaModel.meshes != nullptr) {
        m_arenaModelLoaded = true;
        TraceLog(LOG_INFO, "Arena model loaded successfully");
    } else {
        TraceLog(LOG_WARNING, "Failed to load arena model - using fallback rendering");
        m_arenaModel = (Model){0};
        m_arenaModelLoaded = false;
    }
    
    // Initialize systems
    m_cameraManager = std::make_unique<CameraManager>();
    m_hud = std::make_unique<HUD>();
    m_particleSystem = std::make_unique<ParticleSystem>();
    
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
    Player::UnloadModel();
    Tomato::UnloadModel();
    
    // Unload arena model
    if (m_arenaModelLoaded) {
        UnloadModel(m_arenaModel);
    }
}

void Game::Init() {
    m_player->Reset();
    m_boss->Reset();
    m_cameraManager->Reset();
    m_particleSystem->Clear();
    m_tomatoSpawnTimer = 0.0f;
    m_playerAttackCooldown = 0.0f;
    m_lastPlayerPosition = m_player->GetPosition();
    m_lastBossPosition = m_boss->GetPosition();
    m_playerFootstepTimer = 0.0f;
    m_bossFootstepTimer = 0.0f;
    
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
    
    // Update player rotation to face camera
    m_player->SetCameraAngle(m_cameraManager->GetAngleAroundPlayer());
    
    // Update player with camera-relative movement
    Vector3 cameraForward = m_cameraManager->GetForwardDirection();
<<<<<<< Updated upstream
    Vector3 cameraRight   = m_cameraManager->GetRightDirection();

=======
    Vector3 cameraRight = m_cameraManager->GetRightDirection();
    
    // Handle dash input (Shift key)
    if (IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)) {
        // Get current movement direction
        Vector3 dashDirection = {0, 0, 0};
        
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            dashDirection = Vector3Add(dashDirection, cameraForward);
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            dashDirection = Vector3Subtract(dashDirection, cameraForward);
        }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            dashDirection = Vector3Subtract(dashDirection, cameraRight);
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            dashDirection = Vector3Add(dashDirection, cameraRight);
        }
        
        // If no direction input, dash in the direction player is facing
        if (Vector3Length(dashDirection) == 0) {
            dashDirection = cameraForward;
        }
        
        m_player->StartDash(dashDirection);
    }
    
    // Update player (handles dash movement, timers, gravity)
    m_player->Update(deltaTime);
    
    // Update player with camera-relative movement (skipped if dashing)
>>>>>>> Stashed changes
    m_player->UpdateWithCamera(deltaTime, cameraForward, cameraRight);
    
    // Update boss with player position for smooth rotation
    m_boss->UpdateWithPlayer(m_player->GetPosition(), deltaTime);
    
    // Update particle system
    m_particleSystem->Update(deltaTime);
    
    // Emit particles based on player state
    Vector3 playerPos = m_player->GetPosition();
    
    if (m_player->IsDashing()) {
        // Emit dash particles periodically (not every frame)
        m_dashParticleTimer += deltaTime;
        if (m_dashParticleTimer >= 0.02f) { // Emit every 0.02 seconds during dash
            m_particleSystem->EmitDash(playerPos, Vector3Normalize({cameraForward.x, 0, cameraForward.z}));
            m_dashParticleTimer = 0.0f;
        }
    } else {
        // Reset dash particle timer when not dashing
        m_dashParticleTimer = 0.0f;
        
        // Emit footstep particles when walking (not dashing)
        float playerDist = Vector3Distance(playerPos, m_lastPlayerPosition);
        if (playerDist > 0.1f) { // Player is moving
            m_playerFootstepTimer += deltaTime;
            if (m_playerFootstepTimer >= 0.3f) { // Emit footstep every 0.3 seconds
                m_particleSystem->EmitFootstep(playerPos);
                m_playerFootstepTimer = 0.0f;
            }
        }
    }
    m_lastPlayerPosition = playerPos;
    
    // Update player attack cooldown
    m_playerAttackCooldown -= deltaTime;
    if (m_playerAttackCooldown < 0) m_playerAttackCooldown = 0;
    
    // Toggle camera mode with C key
    if (IsKeyPressed(KEY_C)) {
        m_cameraManager->ToggleMode();
    }
    
    // Toggle cursor lock with ESC key (for debugging or menu access)
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_cameraManager->ToggleCursorLock();
    }
    
    // Toggle boss debug hitbox with H key
    if (IsKeyPressed(KEY_H)) {
        m_boss->ToggleDebugHitbox();
    }
    
    // Handle player attack
    if (IsKeyPressed(KEY_SPACE)) {
        HandlePlayerAttack();
    }
    
    // Handle player projectile attack (Left Mouse Button)
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && m_playerAttackCooldown <= 0) {
        // Shoot projectile toward boss
        for (auto& projectile : m_playerProjectiles) {
            if (!projectile->IsActive()) {
                Vector3 playerPos = m_player->GetPosition();
                Vector3 bossPos = m_boss->GetPosition();
                Vector3 shootDirection = Vector3Normalize(Vector3Subtract(bossPos, playerPos));
                
                projectile->Launch(playerPos, bossPos);
                m_playerAttackCooldown = 1.0f; // Increased cooldown - 1.0 second between shots
                
                // Emit shooting particles
                m_particleSystem->EmitPlayerShoot(playerPos, shootDirection);
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
        HandleBossAttack(bossState);
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
    
    // Draw particle system
    m_particleSystem->Draw();
    
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
        
        // Emit melee attack particles
        Vector3 playerPos = m_player->GetPosition();
        Vector3 bossPos = m_boss->GetPosition();
        Vector3 attackDirection = Vector3Normalize(Vector3Subtract(bossPos, playerPos));
        m_particleSystem->EmitMeleeAttack(playerPos, attackDirection);
    }
}

void Game::HandleBossAttack(BossState attackType) {
    auto& config = GameConfig::GetInstance();
    
    switch (attackType) {
        case BossState::ATTACK_1:
            // Attack 1: Single projectile toward player
            for (auto& projectile : m_projectiles) {
                if (!projectile->IsActive()) {
                    projectile->Launch(m_boss->GetPosition(), m_player->GetPosition());
                    break;
                }
            }
            break;
            
        case BossState::ATTACK_2:
            // Attack 2: Area of Effect - damages player if in range
            {
                Vector3 bossPos = m_boss->GetPosition();
                Vector3 playerPos = m_player->GetPosition();
                float distance = Vector3Distance(bossPos, playerPos);
                float aoeRange = 150.0f; // AoE range
                
                // Emit particle effect for AoE attack
                m_particleSystem->EmitBossAoE(bossPos, aoeRange);
                
                if (distance <= aoeRange && m_player->IsAlive()) {
                    m_player->TakeDamage(config.playerDamagePerHit);
                    printf("Boss ATTACK_2: AoE hit! Distance: %.2f\n", distance);
                }
            }
            break;
            
        case BossState::ATTACK_3:
            // Attack 3: Larger Area of Effect
            {
                Vector3 bossPos = m_boss->GetPosition();
                Vector3 playerPos = m_player->GetPosition();
                float distance = Vector3Distance(bossPos, playerPos);
                float aoeRange = 200.0f; // Larger AoE range
                
                // Emit particle effect for larger AoE attack
                m_particleSystem->EmitBossAoE(bossPos, aoeRange);
                
                if (distance <= aoeRange && m_player->IsAlive()) {
                    m_player->TakeDamage(config.playerDamagePerHit);
                    printf("Boss ATTACK_3: Large AoE hit! Distance: %.2f\n", distance);
                }
            }
            break;
            
        default:
            break;
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
            tomato->Spawn(x, ARENA_FLOOR_Y + TOMATO_RADIUS, z);  // Spawn on arena floor
            break;
        }
    }
}

void Game::DrawArena() const {
    if (m_arenaModelLoaded) {
        // Draw the 3D arena model much lower to account for model's center/top origin
        DrawModel(m_arenaModel, {0.0f, ARENA_MODEL_Y, 0.0f}, 1.0f, WHITE);
    } else {
        // Fallback to simple arena rendering
        DrawPlane({0.0f, ARENA_FLOOR_Y, 0.0f}, {ARENA_SIZE * 2, ARENA_SIZE * 2}, LIGHTGRAY);
        DrawGrid(40, 50.0f);
        
        // Draw arena walls
        DrawCubeWires({0, 50, -ARENA_SIZE}, ARENA_SIZE * 2, 100, 2, DARKGRAY);
        DrawCubeWires({0, 50, ARENA_SIZE}, ARENA_SIZE * 2, 100, 2, DARKGRAY);
        DrawCubeWires({-ARENA_SIZE, 50, 0}, 2, 100, ARENA_SIZE * 2, DARKGRAY);
        DrawCubeWires({ARENA_SIZE, 50, 0}, 2, 100, ARENA_SIZE * 2, DARKGRAY);
    }
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
