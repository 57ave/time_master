# Player Blueprints

## À créer dans Unreal Engine :

### BP_PlayerCharacter
**Type:** Modifier le ThirdPersonCharacter du template ou créer Blueprint Class → Character

**Components:**
- Inherited: Mesh, CapsuleComponent, CharacterMovement
- AttackCollision (Sphere Collision)
  - Location: X=100, Y=0, Z=0
  - Radius: 100
  - Collision: OverlapAllDynamic
  - Hidden in Game: true
  - Start with Collision Enabled: NoCollision

**Variables:**
- bIsAttacking (Boolean) = false
- AttackCooldown (Float) = 0.5
- bCanAttack (Boolean) = true
- AttackDamage (Float) = 5.0 (temps volé au boss)
- Health (Float) = 100.0 (optionnel)

**Fonctions:**
- BeginPlay()
  - Parent: BeginPlay
  - AttackCollision → Set Collision Enabled (NoCollision)
  
- InputAction Attack (Pressed)
  - Branch (bCanAttack)
    - True → PerformAttack()

- PerformAttack()
  - SET bCanAttack = false
  - SET bIsAttacking = true
  - AttackCollision → Set Collision Enabled (QueryOnly)
  - Delay 0.2
  - AttackCollision → Set Collision Enabled (NoCollision)
  - SET bIsAttacking = false
  - Delay (AttackCooldown)
  - SET bCanAttack = true

- OnComponentBeginOverlap (AttackCollision)
  - Other Actor → Cast to BP_BossCharacter
  - Success → Get Game Mode → Cast to BP_TimeBossGameMode
  - Call PlayerHitBoss()
  - Print String "Hit Boss!"

- TakeDamageFromBoss() [PUBLIC]
  - Get Game Mode → Cast to BP_TimeBossGameMode
  - Call BossHitPlayer()
  - Print String "Hit by Boss!" (Red)

**Input Mapping (déjà configuré):**
- Attack = Left Mouse Button + Gamepad Face Button Left

## Notes d'implémentation :

Si vous modifiez le ThirdPersonCharacter existant:
1. Ouvrir Content/ThirdPerson/Blueprints/BP_ThirdPersonCharacter
2. Ajouter les components et variables ci-dessus
3. Implémenter les fonctions dans l'Event Graph

Si vous créez un nouveau BP_PlayerCharacter:
1. Blueprint Class → Character
2. Parent Class peut être ThirdPersonCharacter
3. Hériter des inputs et animations du template
