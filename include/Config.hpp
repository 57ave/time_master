#pragma once

namespace TimeMaster {

// Screen configuration
constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 800;

// Game configuration
constexpr int MAX_TOMATOES = 5;
constexpr int MAX_BOSS_PROJECTILES = 10;
constexpr float ARENA_SIZE = 400.0f;

// Player configuration
constexpr float PLAYER_RADIUS = 20.0f;
constexpr float PLAYER_SPEED = 150.0f;
constexpr float PLAYER_STARTING_TIME = 60.0f;
constexpr float PLAYER_MAX_TIME = 120.0f;
constexpr float PLAYER_DAMAGE_PER_HIT = 5.0f;

// Boss configuration
constexpr float BOSS_WIDTH = 60.0f;
constexpr float BOSS_HEIGHT = 80.0f;
constexpr float BOSS_DEPTH = 60.0f;
constexpr float BOSS_STARTING_TIME = 300.0f;
constexpr float BOSS_DAMAGE_PER_HIT = 3.0f;
constexpr float BOSS_ATTACK_COOLDOWN_MIN = 1.5f;
constexpr float BOSS_ATTACK_COOLDOWN_MAX = 2.5f;

// Tomato configuration
constexpr float TOMATO_RADIUS = 12.0f;
constexpr float TOMATO_LIFETIME = 8.0f;
constexpr float TOMATO_HEAL_AMOUNT = 5.0f;

// Projectile configuration
constexpr float PROJECTILE_RADIUS = 8.0f;
constexpr float PROJECTILE_SPEED = 200.0f;

// Camera configuration
constexpr float CAMERA_DISTANCE = 400.0f;
constexpr float CAMERA_HEIGHT = 300.0f;
constexpr float CAMERA_FOV = 45.0f;

} // namespace TimeMaster
