# Blueprint Node Reference - Complete Implementation

## Visual Flow Diagrams

This document provides ASCII-art style visual representations of the complete Blueprint flows.

## BP_GameMode - Complete Flow

```
┌─────────────────────────────────────────────────────────────┐
│                    BP_GameMode_TimeBattle                   │
├─────────────────────────────────────────────────────────────┤
│ Variables:                                                  │
│  • PlayerTime: 60.0                                         │
│  • BossTime: 300.0                                          │
│  • bGameActive: True                                        │
│  • PlayerRef, BossRef, UIWidgetRef                          │
└─────────────────────────────────────────────────────────────┘

EVENT BeginPlay
    ↓
Get Player Character → Cast to BP_Player → [SUCCESS]
    ↓                                            ↓
Set PlayerRef ←─────────────────────────────────┘
    ↓
Get All Actors (BP_Boss) → Get [0] → Set BossRef
    ↓
Create Widget (WBP_UI) → Set UIWidgetRef
    ↓
Add to Viewport
    ↓
UpdateUI()


FUNCTION PlayerHitBoss()
    ↓
ModifyPlayerTime(+5.0)
    ↓
ModifyBossTime(-5.0)


FUNCTION BossHitPlayer()
    ↓
ModifyPlayerTime(-10.0)
    ↓
ModifyBossTime(+10.0)


FUNCTION ModifyPlayerTime(Amount)
    ↓
PlayerTime = PlayerTime + Amount
    ↓
Clamp(0, 999)
    ↓
UpdateUI()
    ↓
[PlayerTime <= 0?] ──YES──> GameOver("Boss Wins!")
    │
    NO
    ↓
(end)


FUNCTION GameOver(WinnerText)
    ↓
bGameActive = False
    ↓
Print String (WinnerText)
    ↓
Disable Input (PlayerRef)
    ↓
Destroy Actor (BossRef)
    ↓
Delay 3.0s
    ↓
Open Level (Restart)
```

## BP_Player - Complete Flow

```
┌─────────────────────────────────────────────────────────────┐
│                         BP_Player                            │
├─────────────────────────────────────────────────────────────┤
│ Components:                                                  │
│  • AttackCollision (Sphere): X=100, Radius=100              │
│                                                              │
│ Variables:                                                   │
│  • bIsAttacking: False                                       │
│  • AttackCooldown: 0.5                                       │
│  • bCanAttack: True                                          │
└─────────────────────────────────────────────────────────────┘

EVENT BeginPlay
    ↓
Parent::BeginPlay
    ↓
AttackCollision.SetCollisionEnabled(NoCollision)


INPUT Attack (Pressed)
    ↓
[bCanAttack?] ──NO──> (end)
    │
    YES
    ↓
PerformAttack()


FUNCTION PerformAttack()
    ↓
bCanAttack = False
    ↓
bIsAttacking = True
    ↓
Play Anim Montage (optional)
    ↓
AttackCollision.SetCollisionEnabled(QueryOnly) ◄── Hitbox ON
    ↓
Delay 0.2s
    ↓
AttackCollision.SetCollisionEnabled(NoCollision) ◄── Hitbox OFF
    ↓
bIsAttacking = False
    ↓
Delay (AttackCooldown)
    ↓
bCanAttack = True


EVENT AttackCollision.OnOverlap(OtherActor)
    ↓
Cast to BP_Boss
    ↓
[SUCCESS?] ──NO──> (end)
    │
    YES
    ↓
Get Game Mode → Cast to BP_GameMode_TimeBattle
    ↓
Call PlayerHitBoss()
    ↓
Print "Hit Boss!"
    ↓
(Optional) VFX/SFX


FUNCTION TakeDamageFromBoss() ◄── Called by Boss
    ↓
Get Game Mode → Cast to BP_GameMode_TimeBattle
    ↓
Call BossHitPlayer()
    ↓
Print "Hit by Boss!"
    ↓
(Optional) Hurt Animation/Sound
```

## BP_Boss - Complete Flow

```
┌────────────────────────────────────────────────────────────────────┐
│                            BP_Boss                                 │
├────────────────────────────────────────────────────────────────────┤
│ Components:                                                         │
│  • AttackRange (Sphere): Radius=300, Overlap                       │
│  • DamageCollision (Sphere): X=150, Radius=80, Initially Disabled  │
│                                                                     │
│ Variables:                                                          │
│  • CurrentPattern: 0                                                │
│  • AttackPatternArray: [0,1,2,3]                                    │
│  • PatternTimer: 0.0                                                │
│  • PatternDuration: 60.0                                            │
│  • bPlayerInRange: False                                            │
│  • bCanAttack: True                                                 │
│  • AttackCooldown: 2.0                                              │
└────────────────────────────────────────────────────────────────────┘

EVENT BeginPlay
    ↓
Parent::BeginPlay
    ↓
Get Player Character → Cast to BP_Player → Set PlayerRef
    ↓
ShufflePatterns()


EVENT Tick(DeltaTime)
    ↓
[Is Valid PlayerRef?] ──NO──> Get Player Character → Cast → Set PlayerRef
    │
    YES
    ↓
UpdatePatternTimer(DeltaTime)
    ↓
MoveTowardsPlayer()
    ↓
[bPlayerInRange AND bCanAttack?] ──NO──> (end)
    │
    YES
    ↓
ExecuteCurrentPattern()


FUNCTION ShufflePatterns()
    ↓
Clear AttackPatternArray
    ↓
Add 0, 1, 2, 3 to array
    ↓
FOR i = 0 to 10:
    ↓
    Random index A (0-3)
    ↓
    Random index B (0-3)
    ↓
    Swap Array[A] with Array[B]
    ↓
CurrentPattern = Array[0]
    ↓
Print "New Pattern Order"


FUNCTION UpdatePatternTimer(DeltaTime)
    ↓
PatternTimer += DeltaTime
    ↓
[PatternTimer >= 60?] ──NO──> (end)
    │
    YES
    ↓
PatternTimer = 0
    ↓
ShufflePatterns()
    ↓
Print "Boss Pattern Shuffle!"


FUNCTION MoveTowardsPlayer()
    ↓
Get Player Location
    ↓
Get Boss Location
    ↓
Calculate Direction (Player - Boss)
    ↓
Find Look at Rotation → RInterp → Set Actor Rotation
    ↓
Calculate Distance
    ↓
[Distance > 250?] ──NO──> (end - at range)
    │
    YES
    ↓
Normalize Direction
    ↓
Multiply by MoveSpeed
    ↓
Add Movement Input


FUNCTION ExecuteCurrentPattern()
    ↓
SWITCH (CurrentPattern):
    ├─ Case 0: Pattern_FastJabs()
    ├─ Case 1: Pattern_HeavySlam()
    ├─ Case 2: Pattern_SpinAttack()
    └─ Case 3: Pattern_Charge()
    ↓
Advance to next pattern in array


┌──────────────────────────────────────────────────────┐
│                  Attack Patterns                     │
└──────────────────────────────────────────────────────┘

PATTERN_FastJabs:
    ↓
bCanAttack = False
    ↓
Print "Pattern: Fast Jabs!"
    ↓
FOR i = 0 to 2:
    ↓
    Delay (0.3s)
    ↓
    PerformSingleAttack()
    ↓
Delay (AttackCooldown)
    ↓
bCanAttack = True


PATTERN_HeavySlam:
    ↓
bCanAttack = False
    ↓
Print "Pattern: Heavy Slam!"
    ↓
Delay 1.0s (windup)
    ↓
PerformSingleAttack()
    ↓
Delay (AttackCooldown)
    ↓
bCanAttack = True


PATTERN_SpinAttack:
    ↓
bCanAttack = False
    ↓
Print "Pattern: Spin Attack!"
    ↓
FOR i = 0 to 12:
    ↓
    Add Actor Rotation (Yaw: 30°)
    ↓
    Delay 0.05s
    ↓
PerformSingleAttack()
    ↓
Delay (AttackCooldown)
    ↓
bCanAttack = True


PATTERN_Charge:
    ↓
bCanAttack = False
    ↓
Print "Pattern: Charge!"
    ↓
Get Forward Vector × 1000
    ↓
Launch Character
    ↓
Delay 0.5s
    ↓
PerformSingleAttack()
    ↓
Delay (AttackCooldown)
    ↓
bCanAttack = True


FUNCTION PerformSingleAttack()
    ↓
DamageCollision.SetCollisionEnabled(QueryOnly) ◄── Hitbox ON
    ↓
(Optional) Play Attack Animation
    ↓
Delay 0.3s
    ↓
DamageCollision.SetCollisionEnabled(NoCollision) ◄── Hitbox OFF


EVENT AttackRange.OnBeginOverlap(OtherActor)
    ↓
Cast to BP_Player
    ↓
[SUCCESS?] ──NO──> (end)
    │
    YES
    ↓
bPlayerInRange = True
    ↓
Print "Player in range!"


EVENT AttackRange.OnEndOverlap(OtherActor)
    ↓
Cast to BP_Player
    ↓
[SUCCESS?] ──NO──> (end)
    │
    YES
    ↓
bPlayerInRange = False


EVENT DamageCollision.OnBeginOverlap(OtherActor)
    ↓
Cast to BP_Player
    ↓
[SUCCESS?] ──NO──> (end)
    │
    YES
    ↓
PlayerRef.TakeDamageFromBoss()
    ↓
Print "Boss hit player!"
```

## WBP_UI - Layout Structure

```
┌──────────────────────────────────────────────────┐
│              Canvas Panel (Root)                 │
│  ┌────────────────────────────────────────────┐  │
│  │         Vertical Box (Top Center)          │  │
│  │  ┌──────────────────────────────────────┐  │  │
│  │  │     PlayerTimeText (Text Block)      │  │  │
│  │  │   "Player: 60s" - Green, 48pt        │  │  │
│  │  └──────────────────────────────────────┘  │  │
│  │                                            │  │
│  │  ┌──────────────────────────────────────┐  │  │
│  │  │     BossTimeText (Text Block)        │  │  │
│  │  │   "Boss: 300s" - Red, 48pt           │  │  │
│  │  └──────────────────────────────────────┘  │  │
│  └────────────────────────────────────────────┘  │
└──────────────────────────────────────────────────┘

UPDATED BY: GameMode.UpdateUI()
    ↓
Format Text: "Player: {0}s" with PlayerTime
    ↓
Set PlayerTimeText
    ↓
Format Text: "Boss: {0}s" with BossTime
    ↓
Set BossTimeText
```

## Data Flow Overview

```
┌─────────────────────────────────────────────────────────┐
│                   GAME INITIALIZATION                   │
└─────────────────────────────────────────────────────────┘
                         │
                         ↓
              ┌──────────────────────┐
              │   BP_GameMode        │
              │   BeginPlay          │
              └──────────────────────┘
                    │      │      │
        ┌───────────┘      │      └───────────┐
        ↓                  ↓                  ↓
   Find Player        Find Boss          Create UI
        ↓                  ↓                  ↓
   Set PlayerRef      Set BossRef       Set UIWidgetRef
        ↓                  ↓                  ↓
        └──────────────────┴──────────────────┘
                         │
                         ↓
                    UpdateUI()
                         │
                         ↓
              Display Initial Timers


┌─────────────────────────────────────────────────────────┐
│                   COMBAT LOOP (Tick)                    │
└─────────────────────────────────────────────────────────┘

PLAYER SIDE:                    BOSS SIDE:
     │                               │
Input Attack                    UpdatePatternTimer
     ↓                               ↓
PerformAttack                  [60s elapsed?]
     ↓                               ↓
Enable Hitbox                  ShufflePatterns
     ↓                               ↓
[Hit Boss?] ───YES───┐          MoveTowardsPlayer
     │                │              ↓
     NO               │         [In Range + Can Attack?]
     ↓                │              ↓
Disable Hitbox       │         ExecuteCurrentPattern
                     │              ↓
                     │         [Pattern: 0,1,2,3]
                     │              ↓
                     │         Enable Boss Hitbox
                     │              ↓
                     │         [Hit Player?] ───YES───┐
                     │              │                 │
                     │              NO                │
                     │              ↓                 │
                     │         Disable Hitbox        │
                     │                               │
                     ↓                               ↓
            GameMode.PlayerHitBoss()    GameMode.BossHitPlayer()
                     ↓                               ↓
           Player +5s, Boss -5s         Player -10s, Boss +10s
                     ↓                               ↓
                     └───────────┬───────────────────┘
                                 ↓
                            UpdateUI()
                                 ↓
                      [Any timer <= 0?]
                                 ↓
                          YES: GameOver()
                          NO: Continue Loop


┌─────────────────────────────────────────────────────────┐
│                   TIME FLOW (60s)                       │
└─────────────────────────────────────────────────────────┘

Second 0-60:     Pattern Set #1 [e.g., 2,0,3,1]
                 Boss uses pattern 2
                       ↓
Second 60:       ShufflePatterns()
Second 60-120:   Pattern Set #2 [e.g., 3,1,0,2]
                 Boss uses pattern 3
                       ↓
Second 120:      ShufflePatterns()
Second 120-180:  Pattern Set #3 [e.g., 1,3,2,0]
                 Boss uses pattern 1
                       ↓
                 (continues...)
```

## Pin Connection Guide

### Common Pin Types

```
○ White Circle    = Execution (flow control)
● Filled Circle   = Data (values)

Execution Flow:
───○────────────○───  Connect white to white

Data Flow:
───●─Red────────●─Red─────  (Numbers/Float)
───●─Green──────●─Green───  (Boolean)
───●─Blue───────●─Blue────  (Object Reference)
───●─Cyan───────●─Cyan────  (String/Text)
```

### Example Node Connections

```
┌──────────────────┐     ┌──────────────────┐
│  Event BeginPlay │     │   Print String   │
│                  ○─────○                  │
│                  │     │  ●─String        │
└──────────────────┘     └──┼───────────────┘
                            │
                    ┌───────┘
                    │
         ┌──────────┴──────────┐
         │   "Hello World!"    │
         │   (String Literal)  │
         │                   ● │
         └─────────────────────┘
```

## Implementation Time Breakdown

```
Component      | Core Features | Testing | Total
---------------|---------------|---------|--------
WBP_UI         |     10 min    |  5 min  | 15 min
BP_GameMode    |     20 min    | 10 min  | 30 min
BP_Player      |     30 min    | 15 min  | 45 min
BP_Boss        |     60 min    | 30 min  | 90 min
Integration    |       -       | 30 min  | 30 min
---------------|---------------|---------|--------
TOTAL          |    120 min    | 90 min  | 210 min (3.5 hours)
```

## Critical Timing Values

| System | Timing | Purpose |
|--------|--------|---------|
| Player Start Time | 60s | Initial timer |
| Boss Start Time | 300s | Initial timer (5x player) |
| Player Hit Bonus | +5s | Time gained per hit |
| Boss Hit Penalty | -10s | Time lost per hit (2x penalty) |
| Pattern Duration | 60s | Time before shuffle |
| Attack Cooldown | 2s | Boss attack delay |
| Player Cooldown | 0.5s | Player attack delay |
| Attack Active | 0.2s | Hitbox enabled time |
| Game Over Delay | 3s | Restart delay |

These timings are balanced for:
- Boss takes 60 hits to defeat (300/5)
- Player takes 6 hits to lose (60/10)
- Pattern changes 5 times in a full boss fight
- Creates tension with asymmetric risk/reward
