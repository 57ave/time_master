#include "HUD.hpp"
#include "Player.hpp"
#include "Boss.hpp"
#include "Config.hpp"
#include "raymath.h"
#include <cmath>

namespace TimeMaster {

HUD::HUD() : m_fontLoaded(false) {
    // Initialize font to zero
    m_font = {0};
    
    // Load custom font
    if (FileExists("assets/font/Snasm W05 Regular.ttf")) {
        m_font = LoadFont("assets/font/Snasm W05 Regular.ttf");
        if (m_font.texture.id > 0) {
            m_fontLoaded = true;
        }
    }
}

HUD::~HUD() {
    // Font is automatically cleaned up by raylib when CloseWindow() is called
    // Manual unload causes issues with some graphics drivers
}

void HUD::Draw(const Player& player, const Boss& boss) {
    // Draw HUD background
    DrawRectangle(0, 0, SCREEN_WIDTH, 80, Fade(LIGHTGRAY, 0.9f));
    DrawLine(0, 80, SCREEN_WIDTH, 80, BLACK);
    
    // Draw player time
    std::string playerTimeStr = player.GetTimeString();
    DrawTimerDisplay("YOUR TIME:", playerTimeStr, 20, 15, DARKBLUE, 
                    player.GetTime() < 20 ? RED : BLUE);
    
    // Draw player time bar
    auto& config = GameConfig::GetInstance();
    DrawTimeBar(300, 30, player.GetTime(), config.playerMaxTime, SKYBLUE);
    
    // Draw stamina bar below time bar
    DrawTextWithFont("STAMINA:", 300, 55, 16, DARKGREEN);
    float staminaPercent = player.GetStamina() / MAX_STAMINA;
    Color staminaColor = staminaPercent > 0.5f ? GREEN : (staminaPercent > 0.25f ? YELLOW : RED);
    DrawRectangle(380, 55, 200, 15, DARKGRAY);
    DrawRectangle(380, 55, (int)(200 * staminaPercent), 15, staminaColor);
    DrawRectangleLines(380, 55, 200, 15, BLACK);
    
    // Draw boss health as a clock instead of a bar
    DrawTextWithFont("BOSS HP:", SCREEN_WIDTH - 200, 15, 25, RED);
    DrawClockDisplay(SCREEN_WIDTH - 95, 45, boss.GetTime(), config.bossStartingTime, 28);
    
    // Draw controls hint
    DrawTextWithFont("WASD: Move | LMB: Shoot | SPACE: Melee | SHIFT: Dash", 10, SCREEN_HEIGHT - 25, 18, DARKGRAY);
}

void HUD::DrawMenu() {
    DrawTextWithFont("TIME MASTER - BOSS FIGHT (3D)", SCREEN_WIDTH / 2 - 280, 200, 40, DARKBLUE);
    DrawTextWithFont("Defeat the Boss before your time runs out!", SCREEN_WIDTH / 2 - 250, 300, 20, GRAY);
    DrawTextWithFont("CONTROLS:", SCREEN_WIDTH / 2 - 250, 360, 25, DARKGRAY);
    DrawTextWithFont("WASD / Arrow Keys - Move", SCREEN_WIDTH / 2 - 250, 400, 20, BLACK);
    DrawTextWithFont("Left Mouse Button - Shoot Projectile", SCREEN_WIDTH / 2 - 250, 430, 20, BLACK);
    DrawTextWithFont("SPACE - Melee Attack (when close)", SCREEN_WIDTH / 2 - 250, 460, 20, BLACK);
    DrawTextWithFont("SHIFT - Dash (uses stamina)", SCREEN_WIDTH / 2 - 250, 490, 20, BLACK);
    DrawTextWithFont("Collect tomatoes to restore your time!", SCREEN_WIDTH / 2 - 250, 530, 20, RED);
    DrawTextWithFont("Time decreases automatically!", SCREEN_WIDTH / 2 - 250, 560, 20, ORANGE);
    DrawTextWithFont("Press ENTER to Start", SCREEN_WIDTH / 2 - 150, 600, 25, GREEN);
    DrawTextWithFont("Press S for Settings", SCREEN_WIDTH / 2 - 140, 640, 20, BLUE);
}

void HUD::DrawSettings(int selectedOption) {
    auto& config = GameConfig::GetInstance();
    
    DrawTextWithFont("SETTINGS", SCREEN_WIDTH / 2 - 100, 50, 40, DARKBLUE);
    DrawTextWithFont("Use UP/DOWN to select, LEFT/RIGHT to adjust, R to reset", SCREEN_WIDTH / 2 - 300, 110, 18, GRAY);
    
    int startY = 170;
    int spacing = 55;
    int optionIndex = 0;
    
    // Mouse Sensitivity
    Color color0 = (selectedOption == optionIndex) ? RED : BLACK;
    DrawTextWithFont(TextFormat("Mouse Sensitivity: %.2f", config.mouseSensitivity), 
             SCREEN_WIDTH / 2 - 250, startY + spacing * optionIndex, 22, color0);
    optionIndex++;
    
    // Player Speed
    Color color1 = (selectedOption == optionIndex) ? RED : BLACK;
    DrawTextWithFont(TextFormat("Player Speed: %.0f", config.playerSpeed), 
             SCREEN_WIDTH / 2 - 250, startY + spacing * optionIndex, 22, color1);
    optionIndex++;
    
    // Player Starting Time
    Color color2 = (selectedOption == optionIndex) ? RED : BLACK;
    DrawTextWithFont(TextFormat("Player Starting Time: %.0fs", config.playerStartingTime), 
             SCREEN_WIDTH / 2 - 250, startY + spacing * optionIndex, 22, color2);
    optionIndex++;
    
    // Player Max Time
    Color color3 = (selectedOption == optionIndex) ? RED : BLACK;
    DrawTextWithFont(TextFormat("Player Max Time: %.0fs", config.playerMaxTime), 
             SCREEN_WIDTH / 2 - 250, startY + spacing * optionIndex, 22, color3);
    optionIndex++;
    
    // Player Damage
    Color color4 = (selectedOption == optionIndex) ? RED : BLACK;
    DrawTextWithFont(TextFormat("Player Damage: %.1fs", config.playerDamagePerHit), 
             SCREEN_WIDTH / 2 - 250, startY + spacing * optionIndex, 22, color4);
    optionIndex++;
    
    // Boss Starting Time
    Color color5 = (selectedOption == optionIndex) ? RED : BLACK;
    DrawTextWithFont(TextFormat("Boss Starting HP: %.0fs", config.bossStartingTime), 
             SCREEN_WIDTH / 2 - 250, startY + spacing * optionIndex, 22, color5);
    optionIndex++;
    
    // Boss Damage
    Color color6 = (selectedOption == optionIndex) ? RED : BLACK;
    DrawTextWithFont(TextFormat("Boss Damage: %.1fs", config.bossDamagePerHit), 
             SCREEN_WIDTH / 2 - 250, startY + spacing * optionIndex, 22, color6);
    optionIndex++;
    
    // Tomato Heal Amount
    Color color7 = (selectedOption == optionIndex) ? RED : BLACK;
    DrawTextWithFont(TextFormat("Tomato Heal: %.1fs", config.tomatoHealAmount), 
             SCREEN_WIDTH / 2 - 250, startY + spacing * optionIndex, 22, color7);
    optionIndex++;
    
    DrawTextWithFont("Press ESC or ENTER to return to menu", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 60, 20, GREEN);
}

void HUD::DrawGameOver() {
    DrawTextWithFont("GAME OVER", SCREEN_WIDTH / 2 - 150, 300, 50, RED);
    DrawTextWithFont("You ran out of time!", SCREEN_WIDTH / 2 - 130, 370, 25, DARKGRAY);
    DrawTextWithFont("Press ENTER to Retry", SCREEN_WIDTH / 2 - 130, 450, 25, GREEN);
    DrawTextWithFont("Press ESC for Menu", SCREEN_WIDTH / 2 - 120, 490, 20, GRAY);
}

void HUD::DrawVictory() {
    DrawTextWithFont("VICTORY!", SCREEN_WIDTH / 2 - 120, 300, 50, GOLD);
    DrawTextWithFont("You defeated the Boss!", SCREEN_WIDTH / 2 - 140, 370, 25, DARKGRAY);
    DrawTextWithFont("Press ENTER to Play Again", SCREEN_WIDTH / 2 - 150, 450, 25, GREEN);
    DrawTextWithFont("Press ESC for Menu", SCREEN_WIDTH / 2 - 120, 490, 20, GRAY);
}

void HUD::DrawAttackHint(float distance) {
    if (distance < 100) {
        DrawTextWithFont("SPACE for melee! LMB to shoot!", SCREEN_WIDTH / 2 - 120, 85, 20, GREEN);
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
    DrawTextWithFont(label.c_str(), x, y, 25, labelColor);
    DrawTextWithFont(time.c_str(), x, y + 30, 30, timeColor);
}

void HUD::DrawClockDisplay(int x, int y, float current, float max, int radius) {
    // Draw clock circle
    DrawCircle(x, y, radius, Fade(LIGHTGRAY, 0.8f));
    DrawCircleLines(x, y, radius, BLACK);
    
    // Calculate percentage and angle
    float percentage = current / max;
    float angle = percentage * 360.0f;
    
    // Draw colored arc showing remaining time (going counterclockwise from top)
    // Draw segments to create a filled arc
    int segments = 60;
    for (int i = 0; i < segments; i++) {
        float segmentAngle = (angle / segments) * i;
        float startAngle = -90.0f + segmentAngle; // Start at top
        float endAngle = -90.0f + segmentAngle + (angle / segments);
        
        if (i < (int)(percentage * segments)) {
            // Determine color based on percentage
            Color arcColor;
            if (percentage > 0.5f) arcColor = RED;
            else if (percentage > 0.2f) arcColor = ORANGE;
            else arcColor = DARKGRAY;
            
            // Draw arc segment
            DrawCircleSector({(float)x, (float)y}, radius - 2, startAngle, endAngle, 2, arcColor);
        }
    }
    
    // Draw clock hands for visual effect
    // Hour hand (based on percentage)
    float hourAngle = (percentage * 360.0f - 90.0f) * DEG2RAD;
    Vector2 hourEnd = {
        x + cosf(hourAngle) * (radius - 10),
        y + sinf(hourAngle) * (radius - 10)
    };
    DrawLineEx({(float)x, (float)y}, hourEnd, 3, BLACK);
    
    // Minute hand (rotates faster)
    float minuteAngle = (fmodf(current * 6.0f, 360.0f) - 90.0f) * DEG2RAD;
    Vector2 minuteEnd = {
        x + cosf(minuteAngle) * (radius - 5),
        y + sinf(minuteAngle) * (radius - 5)
    };
    DrawLineEx({(float)x, (float)y}, minuteEnd, 2, DARKGRAY);
    
    // Draw center dot
    DrawCircle(x, y, 3, BLACK);
    
    // Draw time text in center
    std::string timeText = TextFormat("%.0f", current);
    int textWidth = MeasureText(timeText.c_str(), 14);
    DrawText(timeText.c_str(), x - textWidth / 2, y + radius + 5, 14, BLACK);
}

void HUD::DrawTextWithFont(const char* text, int x, int y, int fontSize, Color color) {
    if (m_fontLoaded) {
        DrawTextEx(m_font, text, {(float)x, (float)y}, (float)fontSize, 1.0f, color);
    } else {
        DrawText(text, x, y, fontSize, color);
    }
}

} // namespace TimeMaster
