#pragma once
#include "raylib.h"
#include <string>

namespace TimeMaster {

class Player;
class Boss;

/**
 * @brief Renders HUD elements (health bars, time displays, messages)
 */
class HUD {
private:
    Font m_font;
    bool m_fontLoaded;
    
public:
    HUD();
    ~HUD();
    
    /**
     * @brief Draw all HUD elements
     */
    void Draw(const Player& player, const Boss& boss);
    
    /**
     * @brief Draw menu screen
     */
    void DrawMenu();
    
    /**
     * @brief Draw settings screen
     */
    void DrawSettings(int selectedOption);
    
    /**
     * @brief Draw game over screen
     */
    void DrawGameOver();
    
    /**
     * @brief Draw victory screen
     */
    void DrawVictory();
    
    /**
     * @brief Draw attack hint message
     */
    void DrawAttackHint(float distance);
    
private:
    void DrawTimeBar(int x, int y, float current, float max, Color color);
    void DrawTimerDisplay(const std::string& label, const std::string& time, 
                         int x, int y, Color labelColor, Color timeColor);
    void DrawClockDisplay(int x, int y, float current, float max, int radius);
    void DrawTextWithFont(const char* text, int x, int y, int fontSize, Color color);
};

} // namespace TimeMaster
