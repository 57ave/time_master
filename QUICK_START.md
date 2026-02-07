# Quick Start Guide - Time Master Boss Fight

## Project Setup (5 Minutes)

### 1. Create New UE5 Project
1. Epic Games Launcher → Unreal Engine 5.x
2. Games → Third Person Template
3. Blueprint (not C++)
4. No Starter Content needed
5. Name: `TimeMasterBossFight`
6. Create Project

### 2. Implementation Order (Follow in sequence)

**Total Time: ~2-3 hours for basic implementation**

1. **WBP_UI** (15 min) - Start with UI, easiest to test
   - Follow: `WBP_UI_Setup.md`
   - No dependencies

2. **BP_GameMode** (30 min) - Core time management system
   - Follow: `BP_GameMode_Setup.md`
   - Requires WBP_UI to be created first

3. **BP_Player** (45 min) - Player attack system
   - Follow: `BP_Player_Setup.md`
   - Requires BP_GameMode

4. **BP_Boss** (60-90 min) - Most complex: AI + 4 patterns
   - Follow: `BP_Boss_Setup.md`
   - Requires BP_GameMode and BP_Player

### 3. Testing Setup

**Place Boss in Level:**
1. Open Content Drawer
2. Find BP_Boss
3. Drag into level viewport
4. Position away from player start (500-1000 units)

**Play Test:**
1. Click Play (Alt+P)
2. Expected behavior:
   - UI shows: "Player: 60s" (green) and "Boss: 300s" (red)
   - Boss moves toward player
   - Left-click to attack boss
   - Boss attacks back automatically
   - Timers change on hits

## File Structure

```
Content/
├── Blueprints/
│   ├── BP_GameMode_TimeBattle
│   ├── BP_Player
│   └── BP_Boss
└── UI/
    └── WBP_UI
```

## Essential Unreal Editor Skills

### Creating Blueprints
1. Content Drawer → Right-click → Blueprint Class
2. Choose parent class (Character, GameMode, etc.)
3. Name it (BP_NameHere)

### Adding Variables
1. Open Blueprint
2. Variables panel → + icon
3. Set name, type, default value
4. Check "Instance Editable" if needed

### Adding Components
1. Open Blueprint
2. Components panel → + Add
3. Choose component type
4. Rename in Details panel

### Blueprint Nodes
1. Right-click in Event Graph
2. Search for node name
3. Drag connections between pins
4. Output (→) connects to Input

### Input Mapping
1. Edit → Project Settings
2. Engine → Input
3. Action Mappings or Axis Mappings
4. Add new binding

## Common Blueprint Nodes Quick Reference

| Node | Purpose | Common Use |
|------|---------|------------|
| `Event BeginPlay` | Runs once on spawn | Initialize variables |
| `Event Tick` | Runs every frame | Movement, AI updates |
| `Branch` | If/Then logic | Conditionals |
| `Delay` | Wait X seconds | Attack cooldowns |
| `Cast To` | Type conversion | Access specific blueprint |
| `Get/Set` | Variable access | Read/write data |
| `Print String` | Debug text | Testing/debugging |
| `Spawn Actor` | Create object | Spawn projectiles, etc. |
| `Destroy Actor` | Remove object | Cleanup |

## Testing Checklist

### Phase 1: Basic Setup
- [ ] Project created from Third Person template
- [ ] All 4 blueprints created
- [ ] BP_GameMode set as default GameMode
- [ ] BP_Player set as default Pawn in GameMode

### Phase 2: UI Verification
- [ ] WBP_UI shows on screen
- [ ] Player time displays (green, 60s)
- [ ] Boss time displays (red, 300s)

### Phase 3: Player Testing
- [ ] Player can move (WASD)
- [ ] Player can attack (Left-click)
- [ ] Attack has cooldown
- [ ] Hitting boss prints "Hit Boss!"

### Phase 4: Boss Testing
- [ ] Boss placed in level
- [ ] Boss follows player
- [ ] Boss stops at range (~250 units)
- [ ] Boss attacks when in range
- [ ] Boss prints current pattern

### Phase 5: Time System
- [ ] Player hit boss: +5s player, -5s boss
- [ ] Boss hit player: -10s player, +10s boss
- [ ] Timers update on UI immediately
- [ ] Game ends when timer reaches 0

### Phase 6: Pattern System
- [ ] Boss shuffles patterns every 60s
- [ ] "Boss Pattern Shuffle!" prints
- [ ] All 4 patterns execute:
  - [ ] Fast Jabs (3 quick hits)
  - [ ] Heavy Slam (1 heavy hit)
  - [ ] Spin Attack (rotation)
  - [ ] Charge (forward rush)

### Phase 7: Win/Lose Conditions
- [ ] Boss time = 0 → "Player Wins!" → Restart
- [ ] Player time = 0 → "Boss Wins!" → Restart
- [ ] Level restarts after 3 seconds

## Troubleshooting

### "Boss doesn't move"
- Check PlayerRef is set in Boss BeginPlay
- Verify Event Tick is connected
- Check MoveSpeed > 0

### "UI doesn't show"
- Verify WBP_UI created in GameMode BeginPlay
- Check "Add to Viewport" is called
- Ensure UIWidgetRef is valid

### "Attacks don't register"
- Check collision components are present
- Verify collision is enabled during attack
- Ensure Cast to BP_Boss/BP_Player succeeds

### "Timers don't update"
- Check GameMode functions are Public
- Verify Get Game Mode → Cast succeeds
- Ensure UpdateUI is called

### "Patterns don't change"
- Check PatternTimer increments (Event Tick)
- Verify PatternDuration = 60.0
- Ensure ShufflePatterns is called

## Performance Tips

1. **Collision:** Disable when not needed
2. **Tick:** Use timers instead of Tick for periodic tasks
3. **Casting:** Cache references in BeginPlay
4. **Printing:** Remove debug prints before final build

## Next Steps / Polish (Optional)

### Visual Polish
- [ ] Add attack animations
- [ ] Add particle effects (hit/attack)
- [ ] Add sound effects
- [ ] Improve boss mesh/material
- [ ] Add camera shake on hit

### Gameplay Polish
- [ ] Add dodge/block mechanic
- [ ] Vary pattern difficulty
- [ ] Add power-ups
- [ ] Multiple boss phases
- [ ] Add combo system

### UI Polish
- [ ] Add health bar visuals
- [ ] Timer pulse on low time
- [ ] Victory/defeat screens
- [ ] Pattern indicator UI
- [ ] Damage numbers

## Time Estimates

| Task | Minimum | With Polish |
|------|---------|-------------|
| Project Setup | 5 min | 10 min |
| WBP_UI | 15 min | 45 min |
| BP_GameMode | 30 min | 60 min |
| BP_Player | 45 min | 90 min |
| BP_Boss | 90 min | 180 min |
| Testing | 30 min | 60 min |
| **TOTAL** | **~3.5 hours** | **~7 hours** |

## Resources

- UE5 Documentation: https://docs.unrealengine.com/
- Blueprint Visual Scripting: https://docs.unrealengine.com/blueprints
- Third Person Template: Included in UE5

## Support

If stuck:
1. Check individual setup files for detailed steps
2. Use Print String nodes to debug
3. Verify all variables are set correctly
4. Check Blueprint compile errors (top toolbar)
5. Test each component individually before integration

Good luck! Keep it simple and functional first, polish later.
