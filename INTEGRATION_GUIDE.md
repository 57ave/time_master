# Third Person Template Integration Guide

## Overview
Step-by-step guide to integrate the Time Master boss fight system into UE5's Third Person Template.

## Prerequisites
- Unreal Engine 5.0 or higher
- New project from Third Person Template (Blueprint)

## Integration Steps

### Step 1: Project Configuration (5 minutes)

#### 1.1 Open Project Settings
1. `Edit` → `Project Settings`
2. Navigate to settings as indicated below

#### 1.2 Configure Input (if not using default)
**Path:** `Engine` → `Input` → `Action Mappings`

Add new Action Mapping:
- **Name:** `Attack`
- **Bindings:**
  - Left Mouse Button
  - Gamepad Face Button Left

**Result:** Player can attack with mouse/gamepad

#### 1.3 Set Default Classes
**Path:** `Project` → `Maps & Modes`

- **Default GameMode:** `BP_GameMode_TimeBattle`
- **Selected GameMode → Default Pawn Class:** `BP_Player`

**Note:** Set this AFTER creating the blueprints in Step 2.

### Step 2: Create Blueprint Assets (Follow documentation)

#### 2.1 Create Folder Structure
```
Content Browser → Content
└─ Right-click → New Folder → "TimeMaster"
   ├─ New Folder → "Blueprints"
   └─ New Folder → "UI"
```

#### 2.2 Create Blueprints in Order

**In Content/TimeMaster/UI:**
1. Create `WBP_UI` (follow WBP_UI_Setup.md)

**In Content/TimeMaster/Blueprints:**
1. Create `BP_GameMode_TimeBattle` (follow BP_GameMode_Setup.md)
2. Create `BP_Player` (follow BP_Player_Setup.md)
3. Create `BP_Boss` (follow BP_Boss_Setup.md)

### Step 3: Level Setup (10 minutes)

#### 3.1 Open Default Level
1. `File` → `Open Level`
2. Select `ThirdPersonMap` (or your starting map)

#### 3.2 Place Boss in Level
1. Content Browser → Find `BP_Boss`
2. Drag into viewport
3. Position: 
   - Distance from PlayerStart: ~1000 units
   - Make sure it's on the ground
   - Use End key to snap to floor

**Recommended Position:**
- X: 1000, Y: 0, Z: 0 (if player starts at origin)

#### 3.3 Verify Player Start
1. Find `PlayerStart` in World Outliner
2. Ensure it's positioned on solid ground
3. Check rotation faces towards boss area

#### 3.4 Save Level
1. `File` → `Save Current Level`
2. Or `Ctrl+S`

### Step 4: Test Build (5 minutes)

#### 4.1 Initial Play Test
1. Click `Play` button (or Alt+P)
2. Verify:
   - [ ] Player spawns correctly
   - [ ] UI appears with timers
   - [ ] Boss is visible in distance
   - [ ] Can move with WASD
   - [ ] Can attack with Left-click

#### 4.2 Close and Check Console
1. Press `Escape` or click `Stop`
2. Check `Output Log` for errors
3. Common issues: see Troubleshooting section

### Step 5: Game Mode Override (Per Level)

#### Option A: Set for Specific Level
1. `World Settings` panel (Window → World Settings)
2. `GameMode Override` → Select `BP_GameMode_TimeBattle`

#### Option B: Set as Project Default (Recommended)
1. Already done in Step 1.3
2. Applies to all levels

### Step 6: Final Configuration

#### 6.1 Adjust Third Person Camera (Optional)
If camera feels wrong during combat:

**In BP_Player (or ThirdPersonCharacter if you're inheriting):**
1. Select `CameraBoom` component
2. Details panel:
   - Target Arm Length: 400 (default) or adjust
   - Socket Offset Z: 60 (raise camera slightly)
   - Enable Camera Lag: True (smoother)

#### 6.2 Test All Systems
1. Play again
2. Verify complete checklist from QUICK_START.md
3. Test all 4 boss patterns
4. Verify win/lose conditions

### Step 7: Optional Enhancements

#### 7.1 Add Arena Boundaries
1. Place `Box` or `Wall` static meshes
2. Create enclosed combat area
3. Prevents boss/player from falling off

#### 7.2 Improve Lighting
1. Add `Point Light` near combat area
2. Increase `Intensity` to 3000-5000
3. Set `Attenuation Radius` to 1000+

#### 7.3 Add Background Music
1. Import audio file (.wav or .mp3)
2. Create `Sound Cue`
3. In BP_GameMode BeginPlay:
   - `Play Sound 2D` → Your Sound Cue
   - Set `Volume Multiplier` as desired

## Common Third Person Template Modifications

### Using Existing Third Person Character

If you want to extend the existing `ThirdPersonCharacter` instead of creating new `BP_Player`:

#### Method 1: Reparent BP_Player
1. Create `BP_Player` as child of `ThirdPersonCharacter`
2. Follow BP_Player_Setup.md normally
3. Inherit all Third Person movement

#### Method 2: Add to Existing Character
1. Open `ThirdPersonCharacter` blueprint
2. Add all components and logic from BP_Player_Setup.md
3. Rename to `BP_Player` (or update references)

**Recommended:** Method 1 (cleaner, less risk)

### Preserving Third Person Animations

The template includes running/idle animations:
1. These automatically work if using ThirdPersonCharacter as parent
2. Attack animations can be added later
3. Boss can use same animation blueprint or simpler setup

## Project Structure After Integration

```
Content/
├── Characters/           (Template default)
│   ├── Mannequins/
│   └── ...
├── ThirdPerson/          (Template default)
│   ├── Blueprints/
│   ├── Input/
│   └── Maps/
│       └── ThirdPersonMap
└── TimeMaster/           (NEW - Your additions)
    ├── Blueprints/
    │   ├── BP_GameMode_TimeBattle
    │   ├── BP_Player
    │   └── BP_Boss
    └── UI/
        └── WBP_UI
```

## Verification Checklist

After integration, verify:

### Project Settings
- [ ] BP_GameMode_TimeBattle is default GameMode
- [ ] BP_Player is default Pawn
- [ ] Attack input is mapped

### Blueprints
- [ ] All 4 blueprints compile without errors
- [ ] No missing references (pink nodes)
- [ ] All variables have correct types

### Level Setup
- [ ] Boss placed in level
- [ ] PlayerStart exists and is accessible
- [ ] Level saved

### Runtime Testing
- [ ] Game starts without crashes
- [ ] UI displays correctly
- [ ] Player can attack
- [ ] Boss AI functions
- [ ] Time system works
- [ ] Win/lose conditions trigger

## Troubleshooting Integration

### "Player doesn't spawn"
- Check `BP_Player` is set as Default Pawn in GameMode
- Verify PlayerStart exists in level
- Check BP_Player compiles successfully

### "Boss doesn't appear"
- Ensure BP_Boss is placed in level (not just created)
- Check World Outliner for BP_Boss instance
- Verify Z-position is on ground (use End key)

### "UI doesn't show"
- Verify WBP_UI is created in GameMode BeginPlay
- Check Add to Viewport is called
- Ensure Game Mode is active (check World Settings)

### "Template animations don't work on BP_Player"
**Solution:**
1. Open BP_Player
2. Select `Mesh` component
3. Details → `Animation`
4. Anim Class → Set to `ABP_Manny` or `ABP_Quinn`
5. Skeletal Mesh → Set to `SK_Mannequin`

### "Attack input doesn't work"
- Verify Input mapping created
- Check BP_Player has InputAction Attack event
- Ensure Player Controller is set correctly

### "Can't find BP_GameMode in GameMode dropdown"
- Verify blueprint compiles (green checkmark)
- Save all blueprints
- Restart editor if needed
- Check parent class is GameModeBase

## Performance Considerations

### Third Person Template is Optimized
- Uses Level Streaming
- Has collision setup
- Includes proper character movement

### Maintain Performance
1. Don't add excessive Tick events
2. Use timers instead of Tick when possible
3. Disable collision components when not in use
4. Limit print statements in shipping build

### Recommended Settings for Boss Fight
- Max 1 Boss instance
- Player + Boss = 2 characters max
- Use simple collision shapes
- Limit particle effects if performance issues

## Next Steps

After successful integration:
1. Test complete gameplay loop
2. Tune timer values (in GameMode variables)
3. Adjust boss difficulty (pattern timings)
4. Add polish (VFX, SFX, animations)
5. Build standalone game (File → Package Project)

## Building Standalone Game

When ready to share:
1. `File` → `Package Project` → `Windows` (or your platform)
2. Choose output folder
3. Wait for build (5-30 minutes)
4. Test .exe in output folder

**Shipping Settings:**
- Remove all `Print String` nodes (or disable)
- Set Development → Shipping in packaging settings
- Test on different hardware

## Additional Resources

- Template Docs: https://docs.unrealengine.com/Templates
- Character Movement: https://docs.unrealengine.com/character-movement
- Input System: https://docs.unrealengine.com/input

## Support

If stuck during integration:
1. Verify each step was completed
2. Check Blueprint compilation errors
3. Review Output Log for runtime errors
4. Test each component individually
5. Restart Unreal Editor if issues persist

**Common Fix:** Many issues resolve with:
- Compile all blueprints
- Save all
- Restart editor
