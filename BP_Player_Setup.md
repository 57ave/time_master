# BP_Player - Player Character with Attack System

## Overview
Extends the Third Person Character with a simple attack system that interacts with the time-based combat.

## Setup Steps

### 1. Create Player Blueprint
1. Content Browser → Right-click → Blueprint Class → Character
2. Name it `BP_Player`
3. Open the blueprint

### 2. Add Components (Components Panel)

**Add to hierarchy:**
- **AttackCollision** (Sphere Collision)
  - Location: X=100, Y=0, Z=0 (in front of player)
  - Radius: 100
  - Collision Preset: OverlapAllDynamic
  - Hidden in Game: True

### 3. Add Variables

| Variable Name | Type | Default Value | Details |
|--------------|------|---------------|---------|
| `bIsAttacking` | Boolean | False | Is attack active |
| `AttackCooldown` | Float | 0.5 | Time between attacks |
| `bCanAttack` | Boolean | True | Can player attack now |

### 4. Event Graph - Begin Play

```
EVENT BeginPlay
├─ Parent: BeginPlay
└─ SET AttackCollision.SetCollisionEnabled (NoCollision)
```

**Blueprint Nodes:**
1. `Event BeginPlay` → `Parent: BeginPlay`
2. `GET AttackCollision` → `Set Collision Enabled` (No Collision)

### 5. Input Setup

**Project Settings First:**
1. Edit → Project Settings → Input
2. Action Mappings → Add new: `Attack`
   - Mouse: Left Mouse Button
   - Gamepad: Face Button Left (X/Square)

**Event Graph - Attack Input:**
```
EVENT InputAction Attack (Pressed)
└─ Branch (bCanAttack == True)
   └─ TRUE:
      └─ CALL PerformAttack
```

**Blueprint Nodes:**
1. `InputAction Attack` 
2. `Branch` (Condition: bCanAttack)
3. If True: Call `PerformAttack` function

### 6. Custom Function: PerformAttack

**Create Function:**
1. Click + in Functions panel
2. Name: `PerformAttack`

**Function Graph:**
```
FUNCTION PerformAttack
├─ SET bCanAttack = False
├─ SET bIsAttacking = True
├─ Play Anim Montage (Attack Animation if available, or skip)
├─ SET AttackCollision.SetCollisionEnabled (QueryOnly)
├─ DELAY 0.2 seconds
│  └─ SET AttackCollision.SetCollisionEnabled (NoCollision)
├─ SET bIsAttacking = False
└─ DELAY (AttackCooldown)
   └─ SET bCanAttack = True
```

**Blueprint Nodes:**
1. `SET bCanAttack = False`
2. `SET bIsAttacking = True`
3. `Play Anim Montage` (optional, if you have attack animation)
4. `GET AttackCollision` → `Set Collision Enabled` (Query Only)
5. `Delay` (0.2 seconds)
6. `GET AttackCollision` → `Set Collision Enabled` (No Collision)
7. `SET bIsAttacking = False`
8. `Delay` (AttackCooldown seconds)
9. `SET bCanAttack = True`

### 7. Attack Collision Detection

**Event Graph - On Overlap:**
```
EVENT ActorBeginOverlap (AttackCollision)
├─ Other Actor → Cast to BP_Boss
│  └─ SUCCESS:
│     ├─ Get Game Mode → Cast to BP_GameMode_TimeBattle
│     │  └─ CALL PlayerHitBoss
│     ├─ Print String "Hit Boss!" [Duration: 1.0]
│     └─ (Optional) Play Sound at Location / Spawn Emitter
```

**Blueprint Nodes:**
1. Select `AttackCollision` component
2. Details panel → Add Event → `On Component Begin Overlap`
3. `Cast to BP_Boss` (Other Actor)
4. If successful:
   - `Get Game Mode` → `Cast to BP_GameMode_TimeBattle`
   - Call `PlayerHitBoss` function
   - `Print String` "Hit Boss!"
5. (Optional) Add `Play Sound at Location` or `Spawn Emitter at Location`

### 8. Player Taking Damage

**Custom Function: TakeDamageFromBoss**

**Create Function:**
1. Click + in Functions panel
2. Name: `TakeDamageFromBoss`
3. Make it Public (Eye icon open)

**Function Graph:**
```
FUNCTION TakeDamageFromBoss
├─ Get Game Mode → Cast to BP_GameMode_TimeBattle
│  └─ CALL BossHitPlayer
├─ Print String "Hit by Boss!" [Duration: 1.0, Color: Red]
└─ (Optional) Play Hurt Animation / Sound
```

**Blueprint Nodes:**
1. `Get Game Mode` → `Cast to BP_GameMode_TimeBattle`
2. Call `BossHitPlayer` function
3. `Print String` "Hit by Boss!" with Red color
4. (Optional) `Play Anim Montage` for hurt reaction
5. (Optional) `Play Sound 2D` for hurt sound

### 9. Visual Feedback (Optional but Recommended)

**Add to PerformAttack function (after enabling collision):**
```
Spawn Emitter Attached (if you have attack VFX)
├─ Template: P_YourAttackEffect
├─ Attach to: Mesh
└─ Location: Hand Socket or X=50, Y=0, Z=0
```

### 10. Set as Default Pawn

**In BP_GameMode_TimeBattle:**
1. Open BP_GameMode_TimeBattle
2. Class Defaults panel
3. Default Pawn Class = `BP_Player`

## Testing Checklist
- [ ] Player can attack with input (mouse/gamepad)
- [ ] Attack has cooldown (0.5s)
- [ ] Attack collision triggers on boss
- [ ] Hitting boss updates timers (+5s player, -5s boss)
- [ ] Player can take damage from boss
- [ ] Taking damage updates timers (-10s player, +10s boss)

## Quick Node Summary

**Essential Nodes Used:**
- `InputAction` - Detect attack button
- `Set Collision Enabled` - Toggle attack hitbox
- `Delay` - Attack timing and cooldown
- `Cast to BP_Boss` - Identify boss hits
- `Get Game Mode` - Access time manager
- `Branch` - Check if can attack

**Performance Tips:**
- AttackCollision starts disabled
- Only enabled for 0.2s during attack
- Cooldown prevents spam
- Simple overlap detection
