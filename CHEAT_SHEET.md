# Blueprint Cheat Sheet - Quick Reference

## 🎯 At a Glance

### Core Game Loop
```
Player attacks Boss → +5s Player, -5s Boss
Boss attacks Player → -10s Player, +10s Boss
Every 60s → Boss shuffles attack patterns
Timer reaches 0 → Game Over → Restart after 3s
```

## 📊 Key Values Cheat Sheet

| Variable | Location | Default | Purpose |
|----------|----------|---------|---------|
| `PlayerTime` | BP_GameMode | 60.0 | Player starting time |
| `BossTime` | BP_GameMode | 300.0 | Boss starting time |
| `PatternDuration` | BP_Boss | 60.0 | Time before shuffle |
| `AttackCooldown` | BP_Boss | 2.0 | Boss attack delay |
| `AttackCooldown` | BP_Player | 0.5 | Player attack delay |
| `MoveSpeed` | BP_Boss | 300.0 | Boss movement speed |

## 🔧 Component Reference

### BP_GameMode_TimeBattle
**Purpose:** Central time management
**Key Functions:**
- `PlayerHitBoss()` - Call when player hits (+5/-5)
- `BossHitPlayer()` - Call when boss hits (-10/+10)
- `UpdateUI()` - Refresh timer display
- `GameOver(text)` - End game with message

### BP_Player
**Purpose:** Player combat mechanics
**Components:**
- `AttackCollision` (Sphere): X=100, R=100
**Key Functions:**
- `PerformAttack()` - Execute attack
- `TakeDamageFromBoss()` - Receive damage

### BP_Boss
**Purpose:** AI boss with patterns
**Components:**
- `AttackRange` (Sphere): R=300, detect player
- `DamageCollision` (Sphere): X=150, R=80, damage
**Key Functions:**
- `ShufflePatterns()` - Randomize attack order
- `ExecuteCurrentPattern()` - Perform attack
- `Pattern_FastJabs()` - Pattern 0
- `Pattern_HeavySlam()` - Pattern 1
- `Pattern_SpinAttack()` - Pattern 2
- `Pattern_Charge()` - Pattern 3

### WBP_UI
**Purpose:** Display timers
**Variables:**
- `PlayerTimeText` (Text Block) - Green, 48pt
- `BossTimeText` (Text Block) - Red, 48pt

## 🎮 Input Mapping

| Action | Input | Purpose |
|--------|-------|---------|
| Move | WASD | Player movement (inherited) |
| Look | Mouse | Camera control (inherited) |
| Jump | Space | Jump (inherited) |
| **Attack** | **Left Mouse** | **Attack boss** |

## 🔗 Blueprint Communication

### Get Game Mode
```
Get Game Mode → Cast to BP_GameMode_TimeBattle → [use functions]
```

### Get Player
```
Get Player Character → Cast to BP_Player → [use functions]
```

### Find Boss
```
Get All Actors of Class (BP_Boss) → GET [0] → [use]
```

## 📝 Common Node Patterns

### Enable/Disable Collision
```
GET ComponentName
  → Set Collision Enabled
     → NoCollision (off) or QueryOnly (on)
```

### Delay Sequence
```
Action 1
  → Delay (seconds)
     → Action 2
        → Delay (seconds)
           → Action 3
```

### Conditional Execution
```
Branch (Condition)
  ├─ TRUE → Action A
  └─ FALSE → Action B
```

### Timer Update
```
Timer += DeltaTime
  → Branch (Timer >= Duration)
     └─ TRUE:
        ├─ Timer = 0
        └─ Execute Event
```

## 🎨 Color Codes

| Element | Color | Hex | Usage |
|---------|-------|-----|-------|
| Player Timer | Green | #00FF00 | PlayerTimeText |
| Boss Timer | Red | #FF0000 | BossTimeText |
| Warning (optional) | Yellow | #FFFF00 | Low time alert |
| Game Over | Yellow | #FFFF00 | Win/lose message |

## ⏱️ Timing Reference

### Attack Sequences
```
PLAYER ATTACK:
0.0s → Input pressed
0.0s → Collision enabled
0.2s → Collision disabled
0.5s → Can attack again

BOSS FAST JABS:
0.0s → Start
0.3s → Hit 1
0.6s → Hit 2
0.9s → Hit 3
2.9s → Can attack again

BOSS HEAVY SLAM:
0.0s → Start
1.0s → Windup complete
1.0s → Hit
3.0s → Can attack again

BOSS SPIN:
0.0s → Start rotating
0.6s → Complete rotation (12 × 0.05s)
2.6s → Can attack again

BOSS CHARGE:
0.0s → Launch forward
0.5s → Hit check
2.5s → Can attack again
```

## 🛠️ Debugging Commands

### Print Timer Values
```
Event Tick
  → GET PlayerTime
     → Print String (Duration: 0.1)
  → GET BossTime
     → Print String (Duration: 0.1)
```

### Test Win Condition
```
Keyboard Event (F1)
  → SET BossTime = 1.0
     (Boss will lose soon)

Keyboard Event (F2)
  → SET PlayerTime = 1.0
     (Player will lose soon)
```

### Force Pattern Shuffle
```
Keyboard Event (F3)
  → CALL ShufflePatterns
  → SET PatternTimer = 0
```

## 🔍 Troubleshooting Quick Fixes

| Problem | Quick Fix |
|---------|-----------|
| Boss doesn't move | Check `Is Valid (PlayerRef)` |
| UI doesn't show | Verify `Add to Viewport` called |
| Attacks don't hit | Check collision presets: OverlapAllDynamic |
| Timers don't update | Ensure `UpdateUI()` is called |
| Pattern doesn't change | Verify `PatternTimer` increments in Tick |
| Game doesn't end | Check win/lose Branch conditions |

## 📦 File Locations

### Recommended Folder Structure
```
Content/
  TimeMaster/
    Blueprints/
      BP_GameMode_TimeBattle
      BP_Player
      BP_Boss
    UI/
      WBP_UI
```

## 🚀 Quick Implementation Checklist

**5-Minute Test:**
- [ ] Create WBP_UI (just layout)
- [ ] Create BP_GameMode (just variables)
- [ ] Set as default GameMode
- [ ] Play → See if UI shows

**30-Minute MVP:**
- [ ] Complete WBP_UI
- [ ] Complete BP_GameMode with UpdateUI
- [ ] Create basic BP_Player
- [ ] Test time system with Print String

**2-Hour Playable:**
- [ ] Add player attack collision
- [ ] Create BP_Boss with one pattern
- [ ] Wire up all time transfers
- [ ] Test complete game loop

**4-Hour Complete:**
- [ ] All 4 boss patterns
- [ ] Pattern shuffling system
- [ ] Boss AI movement
- [ ] Win/lose conditions
- [ ] Polish and testing

## 📈 Performance Targets

| Metric | Target | Critical |
|--------|--------|----------|
| FPS | 60+ | 30+ |
| Tick Time | <1ms | <5ms |
| Actors in Scene | <10 | <50 |
| Active Collisions | 2-3 | <10 |

## 🎯 Win Condition Math

```
Boss to Win (Player hits):
300s ÷ 5s/hit = 60 hits needed

Player to Lose (Boss hits):
60s ÷ 10s/hit = 6 hits = Game Over

Time Ratio:
Player must hit 10x more often than being hit
(60 player hits vs 6 boss hits max)
```

## 🔢 Balance Formulas

### Adjust Difficulty

**Make Boss Easier:**
- Increase `PatternDuration` (more time to learn)
- Decrease `MoveSpeed` (easier to kite)
- Decrease `BossTime` (fewer hits needed)

**Make Boss Harder:**
- Decrease `AttackCooldown` (attacks faster)
- Increase `MoveSpeed` (harder to escape)
- Increase `BossTime` (more hits needed)

**Make Player Stronger:**
- Increase player hit bonus (>5s)
- Decrease boss hit penalty (<10s)
- Increase `PlayerTime` (more margin for error)

**Make Player Weaker:**
- Decrease player hit bonus (<5s)
- Increase boss hit penalty (>10s)
- Decrease `PlayerTime` (less room for mistakes)

## 🎨 Extension Ideas

### Easy Additions
- Sound effects on hit
- Screen shake on damage
- Particle effects on attack
- Different timer colors when low

### Medium Additions
- Boss phases (multiple patterns)
- Player dodge/block mechanic
- Time pickups in arena
- Combo counter for consecutive hits

### Advanced Additions
- Multiple bosses (wave system)
- Different boss types
- Player upgrades between fights
- Leaderboard (fastest win time)

## 🔗 Quick Links

- **Start Here:** [QUICK_START.md](QUICK_START.md)
- **Integration:** [INTEGRATION_GUIDE.md](INTEGRATION_GUIDE.md)
- **Visual Flows:** [BLUEPRINT_FLOWS.md](BLUEPRINT_FLOWS.md)
- **Full README:** [README.md](README.md)

## 📞 When You're Stuck

1. **Check Blueprint Compilation** - Green checkmark?
2. **Use Print String** - Debug values in real-time
3. **Check Output Log** - Runtime errors shown here
4. **Verify Casting** - Most errors are failed casts
5. **Review Component Setup** - Collision enabled?
6. **Test in Isolation** - One component at a time

## 🎓 Learning Path

**Day 1:** UI + GameMode (understand time system)
**Day 2:** Player (learn collision and input)
**Day 3:** Boss AI (understand AI behavior)
**Day 4:** Patterns (master sequences and timing)
**Day 5:** Polish (VFX, SFX, feel)

---

**Remember:** Keep it simple! Get it working first, polish later! ⏱️
