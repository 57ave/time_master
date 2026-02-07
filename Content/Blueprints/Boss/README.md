# Boss Blueprints

## À créer dans Unreal Engine :

### 1. BP_BossCharacter
**Type:** Blueprint Class → Character

**Components:**
- Mesh (Skeletal ou Static Mesh)
- CapsuleComponent (Collision)
- CharacterMovement
- AttackRange (Sphere Collision)
  - Location: X=0, Y=0, Z=0
  - Radius: 300
  - Collision: OverlapAllDynamic
  - Hidden in Game: true
- DamageCollision (Sphere Collision)
  - Location: X=150, Y=0, Z=0
  - Radius: 80
  - Collision: OverlapAllDynamic
  - Start: NoCollision
  - Hidden in Game: true

**Variables:**
- PlayerRef (Object Reference - BP_PlayerCharacter)
- CurrentPattern (Integer) = 0
- AttackPatternArray (Integer Array) = [0,1,2,3]
- PatternTimer (Float) = 0.0
- PatternDuration (Float) = 60.0
- AttackCooldown (Float) = 2.0
- bCanAttack (Boolean) = true
- bPlayerInRange (Boolean) = false
- MoveSpeed (Float) = 300.0
- AttackDamage (Float) = 10.0
- Health (Float) = 100.0

**Fonctions:**

**BeginPlay()**
```
Parent: BeginPlay
→ Get Player Character → Cast to BP_PlayerCharacter → SET PlayerRef
→ ShufflePatterns()
```

**Event Tick (Delta Time)**
```
Is Valid (PlayerRef)
  → UpdatePatternTimer(DeltaTime)
  → MoveTowardsPlayer()
  → Branch (bPlayerInRange AND bCanAttack)
    → True: ExecuteCurrentPattern()
```

**ShufflePatterns()**
```
CLEAR AttackPatternArray
→ Add 0,1,2,3 to array
→ For Loop (0-10)
  → Random Int (0-3) → IndexA
  → Random Int (0-3) → IndexB
  → Swap array[IndexA] with array[IndexB]
→ SET CurrentPattern = AttackPatternArray[0]
→ Print "Boss shuffled patterns!"
```

**UpdatePatternTimer(DeltaTime)**
```
PatternTimer += DeltaTime
→ Branch (PatternTimer >= PatternDuration)
  → True:
    → SET PatternTimer = 0
    → ShufflePatterns()
```

**MoveTowardsPlayer()**
```
Is Valid (PlayerRef)
  → Get Actor Location (Self)
  → Get Actor Location (Player)
  → Find Look at Rotation
  → RInterp To (smooth rotation)
  → Set Actor Rotation
  → Calculate Distance
  → Branch (Distance > 250)
    → True: Add Movement Input (towards player)
```

**ExecuteCurrentPattern()**
```
Switch on Int (CurrentPattern)
  → 0: Pattern_FastJabs()
  → 1: Pattern_HeavySlam()
  → 2: Pattern_SpinAttack()
  → 3: Pattern_Charge()
→ Increment pattern index
```

**Pattern_FastJabs()** - Pattern 0
```
SET bCanAttack = false
→ For Loop (0-2) - 3 hits
  → DamageCollision → Set Collision Enabled (QueryOnly)
  → Delay 0.15
  → DamageCollision → Set Collision Enabled (NoCollision)
  → Delay 0.3
→ Delay (AttackCooldown)
→ SET bCanAttack = true
```

**Pattern_HeavySlam()** - Pattern 1
```
SET bCanAttack = false
→ Print "Boss Heavy Slam!"
→ Delay 0.5 (wind-up)
→ DamageCollision → Set Collision Enabled (QueryOnly)
→ Delay 0.3
→ DamageCollision → Set Collision Enabled (NoCollision)
→ Delay (AttackCooldown)
→ SET bCanAttack = true
```

**Pattern_SpinAttack()** - Pattern 2
```
SET bCanAttack = false
→ Print "Boss Spin Attack!"
→ DamageCollision → Set Collision Enabled (QueryOnly)
→ For Loop (0-3) - 360° rotation
  → Add Actor Local Rotation (0, 90, 0)
  → Delay 0.2
→ DamageCollision → Set Collision Enabled (NoCollision)
→ Delay (AttackCooldown)
→ SET bCanAttack = true
```

**Pattern_Charge()** - Pattern 3
```
SET bCanAttack = false
→ Print "Boss Charging!"
→ Get direction to Player
→ Launch Character (direction * 1500)
→ DamageCollision → Set Collision Enabled (QueryOnly)
→ Delay 0.8
→ DamageCollision → Set Collision Enabled (NoCollision)
→ Delay (AttackCooldown)
→ SET bCanAttack = true
```

**OnComponentBeginOverlap (DamageCollision)**
```
Other Actor → Cast to BP_PlayerCharacter
  → Success:
    → Call TakeDamageFromBoss()
    → Print "Boss hit player!"
```

**OnComponentBeginOverlap (AttackRange)**
```
Other Actor → Cast to BP_PlayerCharacter
  → Success: SET bPlayerInRange = true
```

**OnComponentEndOverlap (AttackRange)**
```
Other Actor → Cast to BP_PlayerCharacter
  → Success: SET bPlayerInRange = false
```

---

### 2. BP_BossAI (Optionnel - AI Controller)
**Type:** Blueprint Class → AI Controller

Si vous voulez une IA plus avancée avec Behavior Trees, créez ce Blueprint.
Pour l'instant, la logique simple est dans BP_BossCharacter directement.

**Variables:**
- TargetPlayer (Actor Reference)
- CurrentBehavior (Enum ou Integer)

**Fonctions:**
- OnPossess(Pawn)
- UpdateAI()
- FindPlayer()

## Notes :
- Les 4 patterns d'attaque sont dans BP_BossCharacter
- Shuffle automatique toutes les 60 secondes
- Le boss suit le joueur automatiquement
- Attaque quand le joueur est à portée
