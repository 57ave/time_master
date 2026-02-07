#include "Game.hpp"
#include "raymath.h"
#include <cstdlib>
#include <ctime>

namespace TimeMaster {

Game::Game() 
    : m_state(GameState::MENU)
    , m_tomatoSpawnTimer(0.0f) {
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
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
}

Game::~Game() = default;

void Game::Init() {
    m_player->Reset();
    m_boss->Reset();
    m_cameraManager->Reset();
    m_tomatoSpawnTimer = 0.0f;
    
    // Reset all tomatoes and projectiles
    for (auto& tomato : m_tomatoes) {
        // They'll naturally be inactive after construction
    }
    for (auto& projectile : m_projectiles) {
        // They'll naturally be inactive after construction
    }
}

void Game::Update() {
    switch (m_state) {
        case GameState::MENU:
            UpdateMenu();
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
}

void Game::UpdatePlaying() {
    float deltaTime = GetFrameTime();
    
    // Update player
    m_player->Update(deltaTime);
    
    // Handle player attack
    if (IsKeyPressed(KEY_SPACE)) {
        HandlePlayerAttack();
    }
    
    // Update boss
    m_boss->Update(deltaTime);
    
    // Handle boss attack
    if (m_boss->CanAttack()) {
        HandleBossAttack();
        m_boss->ResetAttackCooldown();
    }
    
    // Update projectiles
    UpdateProjectiles(deltaTime);
    
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
    if (m_boss->CheckCollisionWithPlayer(*m_player)) {
        m_boss->TakeDamage(BOSS_DAMAGE_PER_HIT);
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
    for (auto& projectile : m_projectiles) {
        projectile->Update(deltaTime);
        
        if (projectile->CheckCollision(m_player->GetPosition(), m_player->GetRadius())) {
            m_player->TakeDamage(PLAYER_DAMAGE_PER_HIT);
        }
    }
}

void Game::UpdateTomatoes(float deltaTime) {
    for (auto& tomato : m_tomatoes) {
        tomato->Update(deltaTime);
    }
}

void Game::CheckTomatoCollection() {
    for (auto& tomato : m_tomatoes) {
        if (tomato->CheckCollision(m_player->GetPosition(), m_player->GetRadius())) {
            m_player->Heal(TOMATO_HEAL_AMOUNT);
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
}

} // namespace TimeMaster
