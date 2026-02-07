# Time Master - UE5 Blueprint Boss Fight System

A time-based boss fight system for Unreal Engine 5 where health is replaced with time. Every hit steals time from your opponent!

## 🎮 Game Concept

**NO HEALTH BARS - ONLY TIME**
- Player starts with **60 seconds**
- Boss starts with **300 seconds**
- Hit the boss: **steal 5 seconds** (+5s you, -5s boss)
- Get hit by boss: **lose 10 seconds** (-10s you, +10s boss)
- Boss has **4 unique attack patterns** that shuffle every **60 seconds**
- Game ends when either timer hits zero

## 🚀 Quick Start

### For UE5 Developers (Ready to Implement)

1. **Create Project:** UE5 Third Person Template (Blueprint)
2. **Follow Guides in Order:**
   - [`QUICK_START.md`](QUICK_START.md) - Start here!
   - [`INTEGRATION_GUIDE.md`](INTEGRATION_GUIDE.md) - Template integration
   - Individual component guides (see below)
3. **Estimated Time:** 3-4 hours for basic implementation
4. **Complexity:** Beginner to Intermediate

### Documentation Files

| File | Purpose | Time |
|------|---------|------|
| **[QUICK_START.md](QUICK_START.md)** | Fast implementation guide | READ FIRST |
| **[INTEGRATION_GUIDE.md](INTEGRATION_GUIDE.md)** | Third Person Template setup | 15 min |
| **[BP_GameMode_Setup.md](BP_GameMode_Setup.md)** | Time management system | 30 min |
| **[BP_Player_Setup.md](BP_Player_Setup.md)** | Player attack mechanics | 45 min |
| **[BP_Boss_Setup.md](BP_Boss_Setup.md)** | AI boss with 4 patterns | 90 min |
| **[WBP_UI_Setup.md](WBP_UI_Setup.md)** | Dual timer display | 15 min |
| **[BLUEPRINT_FLOWS.md](BLUEPRINT_FLOWS.md)** | Visual flow diagrams | Reference |

## 📋 System Requirements

- **Unreal Engine:** 5.0 or higher (5.1+ recommended)
- **Project Type:** Blueprint (no C++ required)
- **Template:** Third Person Template
- **Platform:** Windows, Mac, Linux (any UE5 platform)
- **Skill Level:** Beginner to Intermediate

## 🎯 Features

### Core Mechanics
- ✅ Time-based combat (no traditional health)
- ✅ Asymmetric risk/reward (+5s vs -10s)
- ✅ Simple attack system (one button)
- ✅ Real-time timer UI display
- ✅ Win/lose conditions with auto-restart

### Boss AI
- ✅ 4 distinct attack patterns:
  1. **Fast Jabs** - Quick 3-hit combo
  2. **Heavy Slam** - Single powerful strike
  3. **Spin Attack** - 360° circular attack
  4. **Charge** - Forward rushing attack
- ✅ Automatic pattern shuffling every 60s
- ✅ Player tracking and movement
- ✅ Range-based attack triggering

### Polish Features (Optional)
- 🔲 Visual effects (VFX)
- 🔲 Sound effects (SFX)
- 🔲 Attack animations
- 🔲 Camera shake on hit
- 🔲 Particle effects
- 🔲 Victory/defeat screens

## 📦 What's Included

All documentation uses **Blueprint node-by-node instructions**:
- Variable setup with exact types and defaults
- Component configuration with transforms
- Event graph flows with visual ASCII diagrams
- Function implementations step-by-step
- Testing checklists for each component
- Troubleshooting sections

## 🛠️ Implementation Overview

### 1. WBP_UI (User Interface)
Simple widget displaying two timers:
- Player time (Green, top)
- Boss time (Red, bottom)
- Auto-updates from GameMode

### 2. BP_GameMode_TimeBattle (Game Manager)
Central time management system:
- Tracks both timers
- Handles time modifications
- Manages win/lose conditions
- Provides public functions for combat

### 3. BP_Player (Player Character)
Third Person character with attack:
- Simple one-button attack
- Sphere collision for hit detection
- Cooldown system (0.5s)
- Communicates with GameMode

### 4. BP_Boss (AI Boss)
Intelligent boss with varied attacks:
- Follows player within range
- 4 unique attack patterns
- Pattern shuffling system
- Attack detection and timing

## 📊 Game Balance

| Metric | Value | Reasoning |
|--------|-------|-----------|
| Player Start Time | 60s | Short, creates urgency |
| Boss Start Time | 300s | 5x player, needs 60 hits |
| Player Gain per Hit | +5s | Steady progress reward |
| Player Loss per Hit | -10s | 2x penalty, high risk |
| Boss Hits to Win | 60 | ~3-5 minute fight |
| Player Hits to Lose | 6 | Forgiveness, but tense |
| Pattern Duration | 60s | Learn and adapt |
| Boss Attack Cooldown | 2s | Dodge opportunity |
| Player Attack Cooldown | 0.5s | Responsive combat |

**Fight Duration:** ~3-5 minutes with skilled play

## 🎓 Learning Blueprint

This project is designed to teach:
- ✅ Blueprint communication (Casting, References)
- ✅ Component-based design
- ✅ Event-driven programming
- ✅ State management
- ✅ Simple AI behavior
- ✅ UI integration
- ✅ Game loops and conditions

**Perfect for:**
- Blueprint beginners wanting a complete project
- Intermediate developers learning UE5
- Rapid prototyping reference
- Game jam foundation (24h target)

## 📖 How to Use This Repository

### For Implementation
1. Read `QUICK_START.md` completely first
2. Create UE5 project from Third Person Template
3. Follow guides in order (UI → GameMode → Player → Boss)
4. Test each component before moving to next
5. Use BLUEPRINT_FLOWS.md for reference

### For Learning
1. Read BLUEPRINT_FLOWS.md to understand architecture
2. Study one component at a time
3. Implement and test incrementally
4. Modify values to see effects
5. Extend with your own features

### For Modification
- All timings are variables (easy to tune)
- Add more attack patterns (extend boss)
- Change time values (balance combat)
- Add power-ups (extend player)
- Multiple boss phases (extend boss)

## 🔧 Customization Guide

### Easy Modifications
```
BP_GameMode Variables:
- PlayerTime: 60.0 → Change starting time
- BossTime: 300.0 → Change boss difficulty
- Win/Lose messages → Custom text

BP_Boss Variables:
- PatternDuration: 60.0 → Pattern shuffle timing
- AttackCooldown: 2.0 → Boss attack speed
- MoveSpeed: 300.0 → Boss movement speed

BP_Player Variables:
- AttackCooldown: 0.5 → Player attack speed
```

### Medium Modifications
- Add new boss attack patterns (extend switch case)
- Change time values per hit (+5/-10)
- Add visual/sound effects
- Implement dodge/block mechanics

### Advanced Modifications
- Multiple boss phases (health gates)
- Power-up system (time boosts)
- Combo system (consecutive hits bonus)
- Difficulty scaling (adaptive AI)

## 🐛 Troubleshooting

Common issues and solutions documented in each guide:
- Boss doesn't move → Check PlayerRef
- UI doesn't show → Verify widget creation
- Attacks don't register → Check collision setup
- Timers don't update → Verify GameMode casting
- Patterns don't shuffle → Check PatternTimer

See QUICK_START.md "Troubleshooting" section for detailed fixes.

## 📈 Development Roadmap

**Core (Implemented via Documentation):**
- [x] Time management system
- [x] Player attack mechanics
- [x] Boss AI with 4 patterns
- [x] Pattern shuffling system
- [x] UI timer display
- [x] Win/lose conditions

**Polish (Optional Extensions):**
- [ ] Visual effects package
- [ ] Sound effects package
- [ ] Animation integration guide
- [ ] Advanced boss patterns
- [ ] Power-up system guide
- [ ] Multiplayer adaptation

## 🤝 Contributing

This is a documentation-focused project. Contributions welcome:
- Additional boss patterns with documentation
- Visual effect examples
- Animation integration guides
- Alternative UI layouts
- Optimization tips
- Video tutorials

## 📄 License

This documentation is provided as-is for educational purposes.
- Use freely in your UE5 projects
- Modify as needed
- Credit appreciated but not required

## 🙏 Acknowledgments

- Built for Unreal Engine 5 Third Person Template
- Designed for rapid prototyping (24h game jams)
- Beginner-friendly Blueprint-only approach
- Community-driven documentation

## 📞 Support

**Documentation Issues:**
- Check QUICK_START.md troubleshooting
- Review individual component guides
- Verify Blueprint compilation
- Check UE5 Output Log

**Need Help?**
- UE5 Documentation: https://docs.unrealengine.com/
- Blueprint Visual Scripting: https://docs.unrealengine.com/blueprints
- Community Forums: https://forums.unrealengine.com/

## 🎮 Ready to Build?

**Start here:** [`QUICK_START.md`](QUICK_START.md)

Estimated total time: **3-4 hours** from zero to playable prototype!

---

**Time Master** - Where every second counts! ⏱️