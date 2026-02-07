# GameSystems Blueprints

## À créer dans Unreal Engine :

### 1. BP_TimeBossGameMode
**Type:** Blueprint Class → Game Mode Base

**Variables:**
- PlayerTime (Float) = 60.0
- BossTime (Float) = 300.0
- bGameActive (Boolean) = true
- PlayerRef (Object Reference - BP_PlayerCharacter)
- BossRef (Object Reference - BP_BossCharacter)
- UIPlayerTimerRef (Object Reference - WBP_PlayerTimer)
- UIBossTimerRef (Object Reference - WBP_BossTimer)

**Fonctions:**
- BeginPlay() - Initialiser les références et UI
- PlayerHitBoss() - Joueur frappe boss (+5s joueur, -5s boss)
- BossHitPlayer() - Boss frappe joueur (-10s joueur, +10s boss)
- ModifyPlayerTime(Amount: Float)
- ModifyBossTime(Amount: Float)
- UpdateTimerUI()
- CheckWinCondition()
- GameOver(Winner: String)

### 2. BP_TimeBossGameState
**Type:** Blueprint Class → Game State Base

**Variables:**
- CurrentPlayerTime (Float)
- CurrentBossTime (Float)
- MatchStartTime (Float)
- bIsMatchActive (Boolean)

**Fonctions:**
- GetPlayerTimeRemaining() → Float
- GetBossTimeRemaining() → Float
- OnRep_PlayerTime() - Pour le multiplayer éventuel
- OnRep_BossTime() - Pour le multiplayer éventuel

## Configuration Project Settings :

1. Edit → Project Settings → Maps & Modes
2. Default GameMode = BP_TimeBossGameMode
3. Selected GameMode → Default Pawn Class = BP_PlayerCharacter
