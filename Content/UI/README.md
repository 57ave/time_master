# UI Widgets

## À créer dans Unreal Engine :

### 1. WBP_PlayerTimer
**Type:** Widget Blueprint

**Hiérarchie:**
```
Canvas Panel
└─ Horizontal Box (Anchored Top-Left)
   ├─ Text Block "Label" - "PLAYER:"
   │  └─ Font: 24pt, Color: White
   └─ Text Block "TimeText" [IS VARIABLE]
      └─ Font: 48pt Bold, Color: Green (#00FF00)
```

**Variables:**
- TimeText (Text Block) - IS VARIABLE
- TimeRemaining (Float) - Bind to GameMode

**Functions:**
- Event Construct
  - (vide, l'update vient du GameMode)

- UpdateTime(NewTime: Float) [PUBLIC]
  - Format Text: "{0}s"
  - SET TimeText
  - Branch (NewTime < 20)
    - True: Color = Yellow
    - False: Color = Green

**Positionnement:**
- Anchors: Top-Left
- Position: X=50, Y=50
- Size: 300x100

---

### 2. WBP_BossTimer
**Type:** Widget Blueprint

**Hiérarchie:**
```
Canvas Panel
└─ Horizontal Box (Anchored Top-Right)
   ├─ Text Block "Label" - "BOSS:"
   │  └─ Font: 24pt, Color: White
   └─ Text Block "TimeText" [IS VARIABLE]
      └─ Font: 48pt Bold, Color: Red (#FF0000)
```

**Variables:**
- TimeText (Text Block) - IS VARIABLE
- TimeRemaining (Float)

**Functions:**
- UpdateTime(NewTime: Float) [PUBLIC]
  - Format Text: "{0}s"
  - SET TimeText
  - Branch (NewTime < 60)
    - True: Color = Orange
    - False: Color = Red

**Positionnement:**
- Anchors: Top-Right
- Position: X=-50, Y=50
- Size: 300x100

---

### 3. WBP_GameOver
**Type:** Widget Blueprint

**Hiérarchie:**
```
Canvas Panel
└─ Overlay (Fill screen, Anchors: Full)
   ├─ Image "Background"
   │  └─ Color: Black, Alpha: 0.7
   └─ Vertical Box (Center)
      ├─ Text Block "WinnerText" [IS VARIABLE]
      │  └─ Font: 72pt Bold, Color: Yellow
      │  └─ Text: "PLAYER WINS!" ou "BOSS WINS!"
      ├─ Spacer (Height: 50)
      ├─ Text Block "SubText"
      │  └─ Font: 36pt, Color: White
      │  └─ Text: "Restarting in 3 seconds..."
      └─ Spacer (Height: 50)
```

**Variables:**
- WinnerText (Text Block) - IS VARIABLE
- SubText (Text Block) - IS VARIABLE
- RestartDelay (Float) = 3.0

**Functions:**
- ShowGameOver(Winner: String, PlayerTime: Float, BossTime: Float)
  - SET WinnerText = Winner
  - Format SubText with times
  - Play Animation (FadeIn)

- Event Construct
  - Delay (RestartDelay)
  - Open Level (Current Level)

**Animations (optionnel):**
- FadeIn: Opacity 0 → 1 over 0.5s
- Pulse: Scale 1.0 → 1.1 → 1.0 loop

**Positionnement:**
- Anchors: Full Screen
- Position: Centered

---

## Intégration dans BP_TimeBossGameMode :

**Event BeginPlay:**
```
Create Widget (WBP_PlayerTimer) → SET UIPlayerTimerRef
→ Add to Viewport

Create Widget (WBP_BossTimer) → SET UIBossTimerRef
→ Add to Viewport
```

**UpdateTimerUI():**
```
Is Valid (UIPlayerTimerRef)
  → Call UpdateTime(PlayerTime)

Is Valid (UIBossTimerRef)
  → Call UpdateTime(BossTime)
```

**GameOver(Winner: String):**
```
Create Widget (WBP_GameOver)
→ Call ShowGameOver(Winner, PlayerTime, BossTime)
→ Add to Viewport
→ Set Input Mode UI Only
→ Show Mouse Cursor
```

## Style Guide :

**Couleurs:**
- Player Timer: Vert #00FF00 (normal), Jaune #FFFF00 (<20s)
- Boss Timer: Rouge #FF0000 (normal), Orange #FFA500 (<60s)
- Game Over: Jaune #FFD700 pour le titre
- Background: Noir avec Alpha 0.7

**Fonts:**
- Labels: 24pt Regular
- Timers: 48pt Bold
- Game Over Title: 72pt Bold
- Game Over Sub: 36pt Regular

**Shadows (optionnel):**
- Offset: X=2, Y=2
- Color: Black, Alpha=0.8
