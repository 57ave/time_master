#include "Game.hpp"
#include "Config.hpp"
#include "raylib.h"

using namespace TimeMaster;

int main() {
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Time Master - Boss Fight (3D)");
    SetTargetFPS(60);
    
    // Create game instance
    Game game;
    
    // Main game loop
    while (!game.ShouldClose()) {
        game.Update();
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        game.Draw();
        EndDrawing();
    }
    
    // Cleanup
    CloseWindow();
    
    return 0;
}
