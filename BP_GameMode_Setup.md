# BP_GameMode - Time Manager Setup

## Overview
The Game Mode manages the time-based combat system, tracking both player and boss timers and handling win/lose conditions.

## Setup Steps

### 1. Create the Blueprint
1. In Content Browser, right-click → Blueprint Class → Game Mode Base
2. Name it `BP_GameMode_TimeBattle`
3. Open the blueprint

### 2. Add Variables (Click + in Variables panel)

| Variable Name | Type | Default Value | Details |
|--------------|------|---------------|---------|
| `PlayerTime` | Float | 60.0 | Player's remaining time |
| `BossTime` | Float | 300.0 | Boss's remaining time |
| `bGameActive` | Boolean | True | Is the game running |
| `PlayerRef` | BP_Player (Object Reference) | None | Reference to player |
| `BossRef` | BP_Boss (Object Reference) | None | Reference to boss |
| `UIWidgetRef` | WBP_UI (Object Reference) | None | Reference to UI widget |

**Set all variables to Instance Editable and exposed on spawn where needed.**

### 3. Event Graph - Begin Play

```
EVENT BeginPlay
├─ Get Player Character
│  └─ Cast to BP_Player
│     ├─ SUCCESS → SET PlayerRef
│     └─ FAIL → Print String "Player not found"
├─ Get All Actors of Class (BP_Boss)
│  └─ GET index [0]
│     └─ SET BossRef
└─ Create Widget (WBP_UI)
   ├─ SET UIWidgetRef
   └─ Add to Viewport
      └─ CALL UpdateUI
```

**Blueprint Nodes:**
1. `Event BeginPlay`
2. `Get Player Character` → `Cast to BP_Player` → `Promote to Variable` (PlayerRef)
3. `Get All Actors of Class` (Class = BP_Boss) → `GET [0]` → `Promote to Variable` (BossRef)
4. `Create Widget` (Class = WBP_UI) → `Promote to Variable` (UIWidgetRef)
5. `Add to Viewport` (Target = UIWidgetRef)
6. `UpdateUI` (custom function - see below)

### 4. Custom Function: UpdateUI

**Create Function:**
1. Click + in Functions panel
2. Name: `UpdateUI`

**Function Graph:**
```
FUNCTION UpdateUI
└─ IS VALID? (UIWidgetRef)
   └─ TRUE:
      ├─ SET UIWidgetRef.PlayerTimeText → (Format: "Player: {0}s" with PlayerTime)
      └─ SET UIWidgetRef.BossTimeText → (Format: "Boss: {0}s" with BossTime)
```

**Blueprint Nodes:**
1. `Is Valid` (check UIWidgetRef)
2. `Format Text` with pattern "Player: {0}s" → input: PlayerTime
3. `Set PlayerTimeText` on UIWidgetRef
4. `Format Text` with pattern "Boss: {0}s" → input: BossTime
5. `Set BossTimeText` on UIWidgetRef

### 5. Custom Function: ModifyPlayerTime

**Create Function:**
1. Click + in Functions panel
2. Name: `ModifyPlayerTime`
3. Add Input: `Amount` (Float)

**Function Graph:**
```
FUNCTION ModifyPlayerTime (Amount)
├─ SET PlayerTime = PlayerTime + Amount
├─ CLAMP PlayerTime (Min: 0, Max: 999)
├─ CALL UpdateUI
└─ Branch (PlayerTime <= 0)
   └─ TRUE → CALL GameOver("Boss Wins!")
```

**Blueprint Nodes:**
1. `GET PlayerTime` → `Float + Float` (add Amount) → `SET PlayerTime`
2. `Clamp (Float)` with Min=0, Max=999
3. `UpdateUI` function call
4. `Branch` (Condition: PlayerTime <= 0)
5. If True: `GameOver` function with "Boss Wins!"

### 6. Custom Function: ModifyBossTime

**Create Function:**
1. Click + in Functions panel
2. Name: `ModifyBossTime`
3. Add Input: `Amount` (Float)

**Function Graph:**
```
FUNCTION ModifyBossTime (Amount)
├─ SET BossTime = BossTime + Amount
├─ CLAMP BossTime (Min: 0, Max: 999)
├─ CALL UpdateUI
└─ Branch (BossTime <= 0)
   └─ TRUE → CALL GameOver("Player Wins!")
```

**Blueprint Nodes:**
1. `GET BossTime` → `Float + Float` (add Amount) → `SET BossTime`
2. `Clamp (Float)` with Min=0, Max=999
3. `UpdateUI` function call
4. `Branch` (Condition: BossTime <= 0)
5. If True: `GameOver` function with "Player Wins!"

### 7. Custom Function: GameOver

**Create Function:**
1. Click + in Functions panel
2. Name: `GameOver`
3. Add Input: `WinnerText` (String)

**Function Graph:**
```
FUNCTION GameOver (WinnerText)
├─ SET bGameActive = False
├─ Print String (WinnerText) [Duration: 10, Color: Yellow]
├─ IS VALID? (PlayerRef)
│  └─ Disable Input (PlayerRef)
├─ IS VALID? (BossRef)
│  └─ Destroy Actor (BossRef)
└─ DELAY 3.0 seconds
   └─ Open Level (Current Level Name) [to restart]
```

**Blueprint Nodes:**
1. `SET bGameActive = False`
2. `Print String` (String = WinnerText, Duration = 10, Text Color = Yellow)
3. `Is Valid` (PlayerRef) → `Disable Input`
4. `Is Valid` (BossRef) → `Destroy Actor`
5. `Delay` (Duration = 3.0)
6. `Get Current Level Name` → `Open Level`

### 8. Public Functions for Player/Boss to Call

**PlayerHitBoss:**
```
FUNCTION PlayerHitBoss
├─ ModifyPlayerTime (+5.0)
└─ ModifyBossTime (-5.0)
```

**BossHitPlayer:**
```
FUNCTION BossHitPlayer
├─ ModifyPlayerTime (-10.0)
└─ ModifyBossTime (+10.0)
```

**Blueprint Nodes:**
1. Create function `PlayerHitBoss` (Public)
   - Call `ModifyPlayerTime` with +5.0
   - Call `ModifyBossTime` with -5.0

2. Create function `BossHitPlayer` (Public)
   - Call `ModifyPlayerTime` with -10.0
   - Call `ModifyBossTime` with +10.0

### 9. Set as Default Game Mode

**Project Settings:**
1. Edit → Project Settings → Maps & Modes
2. Default GameMode = `BP_GameMode_TimeBattle`
3. Save

## Testing Checklist
- [ ] Timers initialize correctly (Player: 60s, Boss: 300s)
- [ ] UI updates when timers change
- [ ] Player wins when Boss time reaches 0
- [ ] Boss wins when Player time reaches 0
- [ ] Level restarts after game over

## Quick Reference

**Get Game Mode in other Blueprints:**
```
Get Game Mode → Cast to BP_GameMode_TimeBattle → (access functions/variables)
```

**Common Calls:**
- `PlayerHitBoss()` - Called when player damages boss
- `BossHitPlayer()` - Called when boss damages player
