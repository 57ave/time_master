# BP_Boss - AI Boss with 4 Attack Patterns

## Overview
AI-controlled boss with 4 attack patterns that shuffle every 60 seconds. Simple but effective combat AI.

## Setup Steps

### 1. Create Boss Blueprint
1. Content Browser → Right-click → Blueprint Class → Character
2. Name it `BP_Boss`
3. Open the blueprint

### 2. Add Components

**Add to hierarchy:**
- **AttackRange** (Sphere Collision)
  - Location: X=0, Y=0, Z=0 (centered on boss)
  - Radius: 300 (attack range)
  - Collision Preset: OverlapAllDynamic
  - Hidden in Game: True

- **DamageCollision** (Sphere Collision)
  - Location: X=150, Y=0, Z=0 (in front of boss)
  - Radius: 80
  - Collision Preset: OverlapAllDynamic
  - Hidden in Game: True
  - Initially set Collision Enabled: NoCollision

### 3. Add Variables

| Variable Name | Type | Default Value | Details |
|--------------|------|---------------|---------|
| `PlayerRef` | BP_Player (Object Reference) | None | Reference to player |
| `CurrentPattern` | Integer | 0 | Current attack pattern (0-3) |
| `AttackPatternArray` | Integer Array | [0,1,2,3] | Shuffled attack order |
| `PatternTimer` | Float | 0.0 | Time in current pattern |
| `PatternDuration` | Float | 60.0 | Time before shuffle (60s) |
| `AttackCooldown` | Float | 2.0 | Time between attacks |
| `bCanAttack` | Boolean | True | Can boss attack |
| `bPlayerInRange` | Boolean | False | Is player in attack range |
| `MoveSpeed` | Float | 300.0 | Boss movement speed |

### 4. Event Graph - Begin Play

```
EVENT BeginPlay
├─ Parent: BeginPlay
├─ Get Player Character → Cast to BP_Player
│  └─ SET PlayerRef
├─ CALL ShufflePatterns
└─ SET Custom Time Dilation = 1.0
```

**Blueprint Nodes:**
1. `Event BeginPlay` → `Parent: BeginPlay`
2. `Get Player Character` → `Cast to BP_Player` → `SET PlayerRef`
3. Call `ShufflePatterns` function (see below)

### 5. Event Tick - AI Logic

```
EVENT Tick (Delta Time)
├─ Branch (IS VALID PlayerRef)
│  └─ TRUE:
│     ├─ CALL UpdatePatternTimer (Delta Time)
│     ├─ CALL MoveTowardsPlayer
│     └─ Branch (bPlayerInRange AND bCanAttack)
│        └─ TRUE → CALL ExecuteCurrentPattern
└─ FALSE → Get Player Character → Cast to BP_Player → SET PlayerRef
```

**Blueprint Nodes:**
1. `Event Tick`
2. `Is Valid` (PlayerRef)
3. If valid:
   - Call `UpdatePatternTimer` (pass Delta Seconds)
   - Call `MoveTowardsPlayer`
   - `Branch` (bPlayerInRange AND bCanAttack)
   - If True: Call `ExecuteCurrentPattern`

### 6. Custom Function: ShufflePatterns

**Create Function:**
1. Functions panel → +
2. Name: `ShufflePatterns`

**Function Graph:**
```
FUNCTION ShufflePatterns
├─ CLEAR AttackPatternArray
├─ Add [0] to AttackPatternArray
├─ Add [1] to AttackPatternArray
├─ Add [2] to AttackPatternArray
├─ Add [3] to AttackPatternArray
├─ ForLoop (0 to 10) - shuffle iterations
│  └─ Random Integer in Range (0, 3) → Index A
│     └─ Random Integer in Range (0, 3) → Index B
│        └─ SWAP AttackPatternArray[Index A] with AttackPatternArray[Index B]
├─ SET CurrentPattern = AttackPatternArray[0]
└─ Print String ("New Pattern Order: ...")
```

**Blueprint Nodes:**
1. `CLEAR` (AttackPatternArray)
2. `Add` (Item=0) to AttackPatternArray
3. `Add` (Item=1) to AttackPatternArray
4. `Add` (Item=2) to AttackPatternArray
5. `Add` (Item=3) to AttackPatternArray
6. `For Loop` (First Index=0, Last Index=10)
   - `Random Integer in Range` (Min=0, Max=3) → A
   - `Random Integer in Range` (Min=0, Max=3) → B
   - `GET` AttackPatternArray[A] → temp
   - `SET` AttackPatternArray[A] = AttackPatternArray[B]
   - `SET` AttackPatternArray[B] = temp
7. `GET` AttackPatternArray[0] → `SET CurrentPattern`
8. `Print String` with pattern info

### 7. Custom Function: UpdatePatternTimer

**Create Function:**
1. Name: `UpdatePatternTimer`
2. Input: `DeltaTime` (Float)

**Function Graph:**
```
FUNCTION UpdatePatternTimer (DeltaTime)
├─ SET PatternTimer = PatternTimer + DeltaTime
└─ Branch (PatternTimer >= PatternDuration)
   └─ TRUE:
      ├─ SET PatternTimer = 0.0
      ├─ CALL ShufflePatterns
      └─ Print String "Boss Pattern Shuffle!" [Duration: 2.0, Color: Orange]
```

**Blueprint Nodes:**
1. `GET PatternTimer` + DeltaTime → `SET PatternTimer`
2. `Branch` (PatternTimer >= PatternDuration)
3. If True:
   - `SET PatternTimer = 0.0`
   - Call `ShufflePatterns`
   - `Print String` "Boss Pattern Shuffle!"

### 8. Custom Function: MoveTowardsPlayer

**Create Function:**
1. Name: `MoveTowardsPlayer`

**Function Graph:**
```
FUNCTION MoveTowardsPlayer
├─ Branch (IS VALID PlayerRef)
│  └─ TRUE:
│     ├─ Get Actor Location (Self)
│     ├─ Get Actor Location (PlayerRef)
│     ├─ Find Look at Rotation (Self → Player)
│     │  └─ SET Actor Rotation (smooth with RInterp)
│     ├─ Vector - Vector (Player Pos - Boss Pos) → Direction
│     ├─ Vector Length (Direction) → Distance
│     └─ Branch (Distance > 250) - stay at attack range
│        └─ TRUE:
│           ├─ Normalize (Direction)
│           ├─ Multiply by MoveSpeed
│           └─ Add Movement Input
```

**Blueprint Nodes:**
1. `Is Valid` (PlayerRef)
2. `Get Actor Location` (Self)
3. `Get Actor Location` (PlayerRef)
4. `Find Look at Rotation` (Start=Self, Target=Player)
5. `RInterp To` (Current Rotation, Target Rotation, Delta=2.0)
6. `Set Actor Rotation`
7. Vector math: PlayerLocation - BossLocation → Direction
8. `Vector Length` → Distance
9. `Branch` (Distance > 250)
10. If True:
    - `Normalize` (Direction)
    - `Multiply` by MoveSpeed
    - `Add Movement Input`

### 9. Custom Function: ExecuteCurrentPattern

**Create Function:**
1. Name: `ExecuteCurrentPattern`

**Function Graph:**
```
FUNCTION ExecuteCurrentPattern
├─ Switch on Int (CurrentPattern)
│  ├─ Case 0: → CALL Pattern_FastJabs
│  ├─ Case 1: → CALL Pattern_HeavySlam
│  ├─ Case 2: → CALL Pattern_SpinAttack
│  └─ Case 3: → CALL Pattern_Charge
└─ Go to next pattern in array
```

**Blueprint Nodes:**
1. `Switch on Int` (Selection = CurrentPattern)
2. Case 0: Call `Pattern_FastJabs`
3. Case 1: Call `Pattern_HeavySlam`
4. Case 2: Call `Pattern_SpinAttack`
5. Case 3: Call `Pattern_Charge`
6. After pattern, move to next in AttackPatternArray (cycle through)

### 10. Attack Pattern Functions

**Pattern_FastJabs (Quick 3-hit combo):**
```
FUNCTION Pattern_FastJabs
├─ SET bCanAttack = False
├─ Print String "Pattern: Fast Jabs!"
├─ For i = 0 to 2:
│  ├─ DELAY (0.3 * i)
│  └─ CALL PerformSingleAttack
└─ DELAY (AttackCooldown)
   └─ SET bCanAttack = True
```

**Pattern_HeavySlam (Single powerful hit):**
```
FUNCTION Pattern_HeavySlam
├─ SET bCanAttack = False
├─ Print String "Pattern: Heavy Slam!"
├─ DELAY 1.0 (windup)
├─ CALL PerformSingleAttack
└─ DELAY (AttackCooldown)
   └─ SET bCanAttack = True
```

**Pattern_SpinAttack (Circular attack):**
```
FUNCTION Pattern_SpinAttack
├─ SET bCanAttack = False
├─ Print String "Pattern: Spin Attack!"
├─ Timeline or Loop:
│  ├─ For i = 0 to 360 step 30
│  │  ├─ Add Actor Local Rotation (0, 30, 0)
│  │  ├─ DELAY 0.05
│  │  └─ Check overlap during spin
│  └─ CALL PerformSingleAttack (at multiple points)
└─ DELAY (AttackCooldown)
   └─ SET bCanAttack = True
```

**Pattern_Charge (Rush forward):**
```
FUNCTION Pattern_Charge
├─ SET bCanAttack = False
├─ Print String "Pattern: Charge!"
├─ Get Forward Vector * 1000 → Launch Velocity
├─ Launch Character (Velocity, XY Override)
├─ DELAY 0.5
├─ CALL PerformSingleAttack
└─ DELAY (AttackCooldown)
   └─ SET bCanAttack = True
```

**Blueprint Nodes for each pattern:**

**Pattern_FastJabs:**
1. `SET bCanAttack = False`
2. `Print String` "Pattern: Fast Jabs!"
3. `For Loop` (0 to 2)
   - `Delay` (0.3 seconds) in sequence
   - Call `PerformSingleAttack`
4. `Delay` (AttackCooldown)
5. `SET bCanAttack = True`

**Pattern_HeavySlam:**
1. `SET bCanAttack = False`
2. `Print String` "Pattern: Heavy Slam!"
3. `Delay` (1.0 windup)
4. Call `PerformSingleAttack`
5. `Delay` (AttackCooldown)
6. `SET bCanAttack = True`

**Pattern_SpinAttack:**
1. `SET bCanAttack = False`
2. `Print String` "Pattern: Spin Attack!"
3. `For Loop` (0 to 12) - 12 steps of 30 degrees
   - `Add Actor Local Rotation` (Yaw=30)
   - `Delay` (0.05)
4. Call `PerformSingleAttack` (check during rotation)
5. `Delay` (AttackCooldown)
6. `SET bCanAttack = True`

**Pattern_Charge:**
1. `SET bCanAttack = False`
2. `Print String` "Pattern: Charge!"
3. `Get Actor Forward Vector` × 1000 → Velocity
4. `Launch Character` (Launch Velocity, XY Override=True, Z Override=False)
5. `Delay` (0.5)
6. Call `PerformSingleAttack`
7. `Delay` (AttackCooldown)
8. `SET bCanAttack = True`

### 11. Custom Function: PerformSingleAttack

**Create Function:**
1. Name: `PerformSingleAttack`

**Function Graph:**
```
FUNCTION PerformSingleAttack
├─ SET DamageCollision.SetCollisionEnabled (QueryOnly)
├─ (Optional) Play Attack Animation
├─ DELAY 0.3
└─ SET DamageCollision.SetCollisionEnabled (NoCollision)
```

**Blueprint Nodes:**
1. `GET DamageCollision` → `Set Collision Enabled` (Query Only)
2. (Optional) `Play Anim Montage`
3. `Delay` (0.3)
4. `GET DamageCollision` → `Set Collision Enabled` (No Collision)

### 12. Collision Events

**AttackRange - Player Detection:**
```
EVENT OnComponentBeginOverlap (AttackRange)
├─ Other Actor → Cast to BP_Player
│  └─ SUCCESS:
│     ├─ SET bPlayerInRange = True
│     └─ Print String "Player in range!"

EVENT OnComponentEndOverlap (AttackRange)
├─ Other Actor → Cast to BP_Player
│  └─ SUCCESS:
│     └─ SET bPlayerInRange = False
```

**DamageCollision - Hit Player:**
```
EVENT OnComponentBeginOverlap (DamageCollision)
├─ Other Actor → Cast to BP_Player
│  └─ SUCCESS:
│     ├─ CALL PlayerRef.TakeDamageFromBoss
│     └─ Print String "Boss hit player!"
```

**Blueprint Nodes:**
1. Select `AttackRange` component → Add Event → On Component Begin Overlap
   - `Cast to BP_Player` (Other Actor)
   - `SET bPlayerInRange = True`

2. Select `AttackRange` component → Add Event → On Component End Overlap
   - `Cast to BP_Player` (Other Actor)
   - `SET bPlayerInRange = False`

3. Select `DamageCollision` component → Add Event → On Component Begin Overlap
   - `Cast to BP_Player` (Other Actor)
   - Call `TakeDamageFromBoss` on PlayerRef
   - `Print String` "Boss hit player!"

### 13. Visual Setup (Mesh)

**In Viewport/Components:**
1. Select `Mesh (Inherited)`
2. In Details:
   - Skeletal Mesh: Pick any humanoid mesh (e.g., SK_Mannequin)
   - OR Static Mesh: Use a simple cube/sphere scaled to ~2x size
   - Material: Set to any contrasting color (Red/Dark)
3. Transform:
   - Scale: 2.0, 2.0, 2.0 (make boss bigger than player)

## Testing Checklist
- [ ] Boss follows player within range
- [ ] Boss stops at attack distance (~250 units)
- [ ] Boss executes attack patterns when in range
- [ ] Pattern shuffles every 60 seconds
- [ ] All 4 patterns work correctly
- [ ] Boss hits player = time transfer works
- [ ] Boss rotation smoothly faces player

## Quick Reference

**4 Attack Patterns:**
1. **Fast Jabs** - 3 quick hits (0.3s apart)
2. **Heavy Slam** - 1 strong hit (1.0s windup)
3. **Spin Attack** - 360° rotating attack
4. **Charge** - Forward rush attack

**Key Timings:**
- Pattern Duration: 60s
- Attack Cooldown: 2s
- Attack Range: 300 units
- Movement Speed: 300 units/s
