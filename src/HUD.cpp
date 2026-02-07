#include "HUD.hpp"
#include "Player.hpp"
#include "Boss.hpp"
#include "Config.hpp"

namespace TimeMaster {

void HUD::Draw(const Player& player, const Boss& boss) {
    // Draw HUD background
    DrawRectangle(0, 0, SCREEN_WIDTH, 80, Fade(LIGHTGRAY, 0.9f));
    DrawLine(0, 80, SCREEN_WIDTH, 80, BLACK);
    
    // Draw player time
    std::string playerTimeStr = player.GetTimeString();
    DrawTimerDisplay("YOUR TIME:", playerTimeStr, 20, 15, DARKBLUE, 
                    player.GetTime() < 20 ? RED : BLUE);
    
    // Draw boss time
    std::string bossTimeStr = boss.GetTimeString();
    DrawTimerDisplay("BOSS HP:", bossTimeStr, SCREEN_WIDTH - 200, 15, RED,
                    boss.GetTime() < 60 ? ORANGE : RED);
    
    // Draw time bars
    DrawTimeBar(300, 30, player.GetTime(), PLAYER_MAX_TIME, SKYBLUE);
    DrawTimeBar(SCREEN_WIDTH - 480, 30, boss.GetTime(), BOSS_STARTING_TIME, MAROON);
    
    // Draw controls hint
    DrawText("WASD/Arrows: Move | SPACE: Attack", 10, SCREEN_HEIGHT - 25, 18, DARKGRAY);
}

void HUD::DrawMenu() {
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

void HUD::DrawGameOver() {
    DrawText("GAME OVER", SCREEN_WIDTH / 2 - 150, 300, 50, RED);
    DrawText("You ran out of time!", SCREEN_WIDTH / 2 - 130, 370, 25, DARKGRAY);
    DrawText("Press ENTER to Retry", SCREEN_WIDTH / 2 - 130, 450, 25, GREEN);
    DrawText("Press ESC for Menu", SCREEN_WIDTH / 2 - 120, 490, 20, GRAY);
}

void HUD::DrawVictory() {
    DrawText("VICTORY!", SCREEN_WIDTH / 2 - 120, 300, 50, GOLD);
    DrawText("You defeated the Boss!", SCREEN_WIDTH / 2 - 140, 370, 25, DARKGRAY);
    DrawText("Press ENTER to Play Again", SCREEN_WIDTH / 2 - 150, 450, 25, GREEN);
    DrawText("Press ESC for Menu", SCREEN_WIDTH / 2 - 120, 490, 20, GRAY);
}

void HUD::DrawAttackHint(float distance) {
    if (distance < 100) {
        DrawText("Press SPACE to attack!", SCREEN_WIDTH / 2 - 100, 85, 20, GREEN);
    }
}

void HUD::DrawTimeBar(int x, int y, float current, float max, Color color) {
    const int width = 250;
    const int height = 30;
    
    float barWidth = (current / max) * width;
    
    DrawRectangle(x, y, width, height, GRAY);
    DrawRectangle(x, y, static_cast<int>(barWidth), height, color);
    DrawRectangleLines(x, y, width, height, BLACK);
}

void HUD::DrawTimerDisplay(const std::string& label, const std::string& time,
                          int x, int y, Color labelColor, Color timeColor) {
    DrawText(label.c_str(), x, y, 25, labelColor);
    DrawText(time.c_str(), x, y + 30, 30, timeColor);
}

} // namespace TimeMaster
