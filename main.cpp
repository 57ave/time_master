#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <string>

constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 800;
constexpr int MAX_TOMATOES = 5;
constexpr int MAX_BOSS_PROJECTILES = 10;
constexpr float ARENA_SIZE = 400.0f;

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    VICTORY
};

class Tomato {
public:
    Vector3 position;
    float radius;
    bool active;
    float lifetime;
    float rotationAngle;
    
    Tomato() : position{0, 0, 0}, radius(12.0f), active(false), lifetime(0.0f), rotationAngle(0.0f) {}
    
    void Spawn(float x, float y, float z) {
        position = {x, y, z};
        active = true;
        lifetime = 8.0f;
        rotationAngle = 0.0f;
    }
    
    void Update(float deltaTime) {
        if (active) {
            lifetime -= deltaTime;
            rotationAngle += deltaTime * 90.0f;
            if (lifetime <= 0) active = false;
        }
    }
    
    void Draw() const {
        if (active) {
            DrawSphere(position, radius, RED);
            DrawSphere({position.x, position.y + radius * 0.8f, position.z}, radius * 0.3f, GREEN);
        }
    }
    
    bool CheckCollision(Vector3 pos, float otherRadius) const {
        if (!active) return false;
        return Vector3Distance(position, pos) < radius + otherRadius;
    }
};

class Projectile {
public:
    Vector3 position;
    Vector3 velocity;
    float radius;
    bool active;
    
    Projectile() : position{0, 0, 0}, velocity{0, 0, 0}, radius(8.0f), active(false) {}
    
    void Launch(Vector3 startPos, Vector3 targetPos) {
        position = startPos;
        Vector3 direction = Vector3Subtract(targetPos, startPos);
        direction = Vector3Normalize(direction);
        velocity = Vector3Scale(direction, 200.0f);
        active = true;
    }
    
    void Update(float deltaTime) {
        if (active) {
            position = Vector3Add(position, Vector3Scale(velocity, deltaTime));
            if (fabs(position.x) > ARENA_SIZE || position.y < 0 || position.y > 200 ||
                fabs(position.z) > ARENA_SIZE) {
                active = false;
            }
        }
    }
    
    void Draw() const {
        if (active) {
            DrawSphere(position, radius, ORANGE);
        }
    }
    
    bool CheckCollision(Vector3 pos, float otherRadius) {
        if (!active) return false;
        if (Vector3Distance(position, pos) < radius + otherRadius) {
            active = false;
            return true;
        }
        return false;
    }
};

class Player {
public:
    Vector3 position;
    float radius;
    float speed;
    float playerTime;
    bool isAlive;
    
    Player() { Reset(); }
    
    void Reset() {
        position = {-200, 30, 0};
        radius = 20.0f;
        speed = 150.0f;
        playerTime = 60.0f;
        isAlive = true;
    }
    
    void Update(float deltaTime) {
        Vector3 movement = {0, 0, 0};
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) movement.z -= 1;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) movement.z += 1;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) movement.x -= 1;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) movement.x += 1;
        
        if (Vector3Length(movement) > 0) {
            movement = Vector3Normalize(movement);
            position = Vector3Add(position, Vector3Scale(movement, speed * deltaTime));
        }
        
        position.x = Clamp(position.x, -ARENA_SIZE + radius, ARENA_SIZE - radius);
        position.z = Clamp(position.z, -ARENA_SIZE + radius, ARENA_SIZE - radius);
        position.y = 30.0f;
    }
    
    void TakeDamage(float damage) {
        playerTime -= damage;
        if (playerTime < 0) playerTime = 0;
    }
    
    void Heal(float amount) {
        playerTime += amount;
        if (playerTime > 120.0f) playerTime = 120.0f;
    }
    
    void Draw() const {
        DrawSphere(position, radius, BLUE);
        DrawSphereWires(position, radius + 1, 8, 8, DARKBLUE);
    }
    
    std::string GetTimeString() const {
        int minutes = static_cast<int>(playerTime / 60);
        int seconds = static_cast<int>(playerTime) % 60;
        char buffer[32];
        sprintf(buffer, "%d:%02d", minutes, seconds);
        return std::string(buffer);
    }
};

class Boss {
public:
    Vector3 position;
    float width, height, depth;
    float bossTime;
    bool isAlive;
    float attackCooldown;
    float moveTimer;
    Vector3 velocity;
    
    Boss() { Reset(); }
    
    void Reset() {
        position = {200, 80, 0};
        width = 60.0f;
        height = 80.0f;
        depth = 60.0f;
        bossTime = 300.0f;
        isAlive = true;
        attackCooldown = 0.0f;
        moveTimer = 0.0f;
        velocity = {0, 0, 30};
    }
    
    void Update(float deltaTime) {
        moveTimer += deltaTime;
        position = Vector3Add(position, Vector3Scale(velocity, deltaTime));
        
        if (position.z <= -ARENA_SIZE / 2 + depth / 2 || position.z >= ARENA_SIZE / 2 - depth / 2) {
            velocity.z *= -1;
        }
        if (position.x <= -ARENA_SIZE / 2 + width / 2 || position.x >= ARENA_SIZE / 2 - width / 2) {
            velocity.x *= -1;
        }
        
        if (moveTimer > 2.0f) {
            moveTimer = 0;
            if (GetRandomValue(0, 1)) velocity.z *= -1;
            if (GetRandomValue(0, 1)) velocity.x = static_cast<float>(GetRandomValue(-20, 20));
        }
        
        attackCooldown -= deltaTime;
    }
    
    void TakeDamage(float damage) {
        bossTime -= damage;
        if (bossTime < 0) {
            bossTime = 0;
            isAlive = false;
        }
    }
    
    bool CanAttack() const { return attackCooldown <= 0; }
    
    void ResetAttackCooldown() {
        attackCooldown = 1.5f + static_cast<float>(GetRandomValue(0, 10)) / 10.0f;
    }
    
    void Draw() const {
        DrawCube(position, width, height, depth, RED);
        DrawCubeWires(position, width, height, depth, DARKBROWN);
        
        Vector3 leftEye = {position.x - width * 0.2f, position.y + height * 0.2f, position.z - depth * 0.51f};
        Vector3 rightEye = {position.x + width * 0.2f, position.y + height * 0.2f, position.z - depth * 0.51f};
        DrawSphere(leftEye, 5.0f, BLACK);
        DrawSphere(rightEye, 5.0f, BLACK);
        DrawSphere(leftEye, 2.5f, YELLOW);
        DrawSphere(rightEye, 2.5f, YELLOW);
    }
    
    std::string GetTimeString() const {
        int minutes = static_cast<int>(bossTime / 60);
        int seconds = static_cast<int>(bossTime) % 60;
        char buffer[32];
        sprintf(buffer, "%d:%02d", minutes, seconds);
        return std::string(buffer);
    }
    
    bool CheckCollisionWithPlayer(const Player& player) const {
        BoundingBox box = {
            {position.x - width / 2, position.y - height / 2, position.z - depth / 2},
            {position.x + width / 2, position.y + height / 2, position.z + depth / 2}
        };
        return CheckCollisionBoxSphere(box, player.position, player.radius);
    }
};

class Game {
private:
    Player player;
    Boss boss;
    std::vector<Tomato> tomatoes;
    std::vector<Projectile> projectiles;
    GameState state;
    Camera3D camera;
    
public:
    Game() : tomatoes(MAX_TOMATOES), projectiles(MAX_BOSS_PROJECTILES), state(GameState::MENU) {
        srand(static_cast<unsigned int>(time(nullptr)));
        camera.position = {-400.0f, 300.0f, 400.0f};
        camera.target = {0.0f, 50.0f, 0.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
    }
    
    void Init() {
        player.Reset();
        boss.Reset();
        for (auto& t : tomatoes) t.active = false;
        for (auto& p : projectiles) p.active = false;
    }
    
    void Update() {
        if (state == GameState::MENU) {
            if (IsKeyPressed(KEY_ENTER)) {
                Init();
                state = GameState::PLAYING;
            }
        } else if (state == GameState::PLAYING) {
            UpdatePlaying();
        } else if (state == GameState::GAME_OVER || state == GameState::VICTORY) {
            if (IsKeyPressed(KEY_ENTER)) {
                Init();
                state = GameState::PLAYING;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                state = GameState::MENU;
            }
        }
    }
    
    void UpdatePlaying() {
        float deltaTime = GetFrameTime();
        
        player.Update(deltaTime);
        
        if (IsKeyPressed(KEY_SPACE)) {
            if (boss.CheckCollisionWithPlayer(player)) {
                boss.TakeDamage(3.0f);
                if (!boss.isAlive) state = GameState::VICTORY;
            }
        }
        
        boss.Update(deltaTime);
        
        if (boss.CanAttack()) {
            for (auto& p : projectiles) {
                if (!p.active) {
                    p.Launch(boss.position, player.position);
                    break;
                }
            }
            boss.ResetAttackCooldown();
        }
        
        for (auto& p : projectiles) {
            p.Update(deltaTime);
            if (p.CheckCollision(player.position, player.radius)) {
                player.TakeDamage(5.0f);
            }
        }
        
        for (auto& t : tomatoes) t.Update(deltaTime);
        
        if (GetRandomValue(0, 180) == 0) SpawnTomato();
        
        for (auto& t : tomatoes) {
            if (t.CheckCollision(player.position, player.radius)) {
                player.Heal(5.0f);
                t.active = false;
            }
        }
        
        if (player.playerTime <= 0) {
            player.isAlive = false;
            state = GameState::GAME_OVER;
        }
    }
    
    void SpawnTomato() {
        for (auto& t : tomatoes) {
            if (!t.active) {
                float x = static_cast<float>(GetRandomValue(-ARENA_SIZE + 50, ARENA_SIZE - 50));
                float z = static_cast<float>(GetRandomValue(-ARENA_SIZE + 50, ARENA_SIZE - 50));
                t.Spawn(x, 15.0f, z);
                break;
            }
        }
    }
    
    void Draw() {
        if (state == GameState::MENU) {
            DrawMenu();
        } else if (state == GameState::PLAYING) {
            DrawPlaying();
        } else if (state == GameState::GAME_OVER) {
            DrawGameOver();
        } else if (state == GameState::VICTORY) {
            DrawVictory();
        }
    }
    
    void DrawMenu() {
        DrawText("TIME MASTER - BOSS FIGHT (3D)", SCREEN_WIDTH / 2 - 280, 200, 40, DARKBLUE);
        DrawText("Defeat the Boss before your time runs out!", SCREEN_WIDTH / 2 - 250, 300, 20, GRAY);
        DrawText("CONTROLS:", SCREEN_WIDTH / 2 - 250, 360, 25, DARKGRAY);
        DrawText("WASD / Arrow Keys - Move", SCREEN_WIDTH / 2 - 250, 400, 20, BLACK);
        DrawText("SPACE - Attack Boss (when close)", SCREEN_WIDTH / 2 - 250, 430, 20, BLACK);
        DrawText("Collect tomatoes to restore your time!", SCREEN_WIDTH / 2 - 250, 470, 20, RED);
        DrawText("Each hit reduces boss time by 3 seconds", SCREEN_WIDTH / 2 - 250, 500, 20, DARKGREEN);
        DrawText("Boss attacks reduce your time by 5 seconds", SCREEN_WIDTH / 2 - 250, 530, 20, ORANGE);
        DrawText("Press ENTER to Start", SCREEN_WIDTH / 2 - 150, 600, 25, GREEN);
    }
    
    void DrawGameOver() {
        DrawText("GAME OVER", SCREEN_WIDTH / 2 - 150, 300, 50, RED);
        DrawText("You ran out of time!", SCREEN_WIDTH / 2 - 130, 370, 25, DARKGRAY);
        DrawText("Press ENTER to Retry", SCREEN_WIDTH / 2 - 130, 450, 25, GREEN);
        DrawText("Press ESC for Menu", SCREEN_WIDTH / 2 - 120, 490, 20, GRAY);
    }
    
    void DrawVictory() {
        DrawText("VICTORY!", SCREEN_WIDTH / 2 - 120, 300, 50, GOLD);
        DrawText("You defeated the Boss!", SCREEN_WIDTH / 2 - 140, 370, 25, DARKGRAY);
        DrawText("Press ENTER to Play Again", SCREEN_WIDTH / 2 - 150, 450, 25, GREEN);
        DrawText("Press ESC for Menu", SCREEN_WIDTH / 2 - 120, 490, 20, GRAY);
    }
    
    void DrawPlaying() {
        BeginMode3D(camera);
        
        // Draw arena floor
        DrawPlane({0.0f, 0.0f, 0.0f}, {ARENA_SIZE * 2, ARENA_SIZE * 2}, LIGHTGRAY);
        DrawGrid(40, 50.0f);
        
        // Draw arena walls
        DrawCubeWires({0, 50, -ARENA_SIZE}, ARENA_SIZE * 2, 100, 2, DARKGRAY);
        DrawCubeWires({0, 50, ARENA_SIZE}, ARENA_SIZE * 2, 100, 2, DARKGRAY);
        DrawCubeWires({-ARENA_SIZE, 50, 0}, 2, 100, ARENA_SIZE * 2, DARKGRAY);
        DrawCubeWires({ARENA_SIZE, 50, 0}, 2, 100, ARENA_SIZE * 2, DARKGRAY);
        
        player.Draw();
        boss.Draw();
        for (const auto& t : tomatoes) t.Draw();
        for (const auto& p : projectiles) p.Draw();
        
        EndMode3D();
        
        // Draw HUD
        DrawRectangle(0, 0, SCREEN_WIDTH, 80, Fade(LIGHTGRAY, 0.9f));
        DrawLine(0, 80, SCREEN_WIDTH, 80, BLACK);
        
        std::string playerTimeStr = player.GetTimeString();
        DrawText("YOUR TIME:", 20, 15, 25, DARKBLUE);
        DrawText(playerTimeStr.c_str(), 20, 45, 30, player.playerTime < 20 ? RED : BLUE);
        
        std::string bossTimeStr = boss.GetTimeString();
        DrawText("BOSS HP:", SCREEN_WIDTH - 200, 15, 25, RED);
        DrawText(bossTimeStr.c_str(), SCREEN_WIDTH - 200, 45, 30, boss.bossTime < 60 ? ORANGE : RED);
        
        float playerBarWidth = (player.playerTime / 120.0f) * 250;
        DrawRectangle(300, 30, 250, 30, GRAY);
        DrawRectangle(300, 30, static_cast<int>(playerBarWidth), 30, SKYBLUE);
        DrawRectangleLines(300, 30, 250, 30, BLACK);
        
        float bossBarWidth = (boss.bossTime / 300.0f) * 250;
        DrawRectangle(SCREEN_WIDTH - 480, 30, 250, 30, GRAY);
        DrawRectangle(SCREEN_WIDTH - 480, 30, static_cast<int>(bossBarWidth), 30, MAROON);
        DrawRectangleLines(SCREEN_WIDTH - 480, 30, 250, 30, BLACK);
        
        float distToBoss = Vector3Distance(player.position, boss.position);
        if (distToBoss < 100) {
            DrawText("Press SPACE to attack!", SCREEN_WIDTH / 2 - 100, 85, 20, GREEN);
        }
        
        DrawText("WASD/Arrows: Move | SPACE: Attack", 10, SCREEN_HEIGHT - 25, 18, DARKGRAY);
    }
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Time Master - Boss Fight (3D)");
    SetTargetFPS(60);
    
    Game game;
    
    while (!WindowShouldClose()) {
        game.Update();
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        game.Draw();
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
