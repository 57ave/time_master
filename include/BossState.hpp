#pragma once

namespace TimeMaster {

/**
 * @brief Boss animation states
 */
enum class BossState {
    IDLE,       // Resting/waiting state
    ATTACK_1,   // First attack pattern
    ATTACK_2,   // Second attack pattern
    ATTACK_3,   // Third attack pattern
    DEATH       // Death animation (for later)
};

} // namespace TimeMaster
