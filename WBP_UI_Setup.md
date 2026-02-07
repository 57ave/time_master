# WBP_UI - Dual Timer User Interface

## Overview
Simple UI widget displaying both player and boss timers with clear visual hierarchy.

## Setup Steps

### 1. Create Widget Blueprint
1. Content Browser → Right-click → User Interface → Widget Blueprint
2. Name it `WBP_UI`
3. Open the widget designer

### 2. Widget Hierarchy (Designer)

```
Canvas Panel (Root)
└─ Vertical Box (Fill Screen)
   ├─ Text Block - "PlayerTimeText"
   │  └─ Size: 48pt, Color: Green, Alignment: Center
   └─ Text Block - "BossTimeText"
      └─ Size: 48pt, Color: Red, Alignment: Center
```

**Step-by-step Designer Setup:**

1. **Canvas Panel** (should already exist as root)

2. **Add Vertical Box:**
   - Palette → Panel → Vertical Box
   - Drag to Canvas Panel
   - Anchors: Top-Center (or Full Screen)
   - Position: X=0, Y=50 (top of screen)
   - Size to Content: True

3. **Add PlayerTimeText (Text Block):**
   - Palette → Common → Text (Text Block)
   - Drag into Vertical Box
   - Name it: `PlayerTimeText` (in Details panel)
   - Text: "Player: 60s" (default)
   - Font Size: 48
   - Color: Green (#00FF00)
   - Justification: Align Text Center
   - Make Variable: ✓ (Check "Is Variable")

4. **Add BossTimeText (Text Block):**
   - Palette → Common → Text (Text Block)
   - Drag into Vertical Box (below PlayerTimeText)
   - Name it: `BossTimeText` (in Details panel)
   - Text: "Boss: 300s" (default)
   - Font Size: 48
   - Color: Red (#FF0000)
   - Justification: Align Text Center
   - Make Variable: ✓ (Check "Is Variable")

### 3. Detailed Widget Settings

**PlayerTimeText Settings:**
| Property | Value |
|----------|-------|
| Text | "Player: 60s" |
| Font → Size | 48 |
| Color and Opacity | (R=0, G=1, B=0, A=1) Green |
| Font → Typeface | Bold or Roboto Bold |
| Justification | Center |
| Shadow Offset | X=2, Y=2 (optional) |
| Shadow Color | Black with Opacity 0.5 |

**BossTimeText Settings:**
| Property | Value |
|----------|-------|
| Text | "Boss: 300s" |
| Font → Size | 48 |
| Color and Opacity | (R=1, G=0, B=0, A=1) Red |
| Font → Typeface | Bold or Roboto Bold |
| Justification | Center |
| Shadow Offset | X=2, Y=2 (optional) |
| Shadow Color | Black with Opacity 0.5 |

**Vertical Box Settings:**
| Property | Value |
|----------|-------|
| Padding | Top: 20, Bottom: 20 |
| Size to Content | True |

### 4. Advanced Layout (Optional)

**For better visual separation, add Spacers:**

```
Vertical Box
├─ Spacer (Height: 20)
├─ PlayerTimeText
├─ Spacer (Height: 40)
└─ BossTimeText
```

**Add Spacer:**
1. Palette → Primitive → Spacer
2. Drag into Vertical Box
3. Size: Fill, Size Rule: Fill

### 5. Alternative Layout - Side by Side

**If you prefer horizontal layout:**

```
Canvas Panel
└─ Horizontal Box
   ├─ PlayerTimeText (Left)
   └─ BossTimeText (Right)
```

**Horizontal Box Setup:**
1. Replace Vertical Box with Horizontal Box
2. Anchor to Top-Stretch
3. Same text settings as above
4. Add padding between: Spacer in middle

### 6. Graph (Blueprint) - Optional Updates

**If you want to add color changes based on time:**

**Event Construct (runs once):**
```
EVENT Construct
└─ (Nothing needed - GameMode handles updates)
```

**Optional Custom Function: UpdateTimerColors**
```
FUNCTION UpdateTimerColors (PlayerTime, BossTime)
├─ Branch (PlayerTime < 20)
│  └─ TRUE → SET PlayerTimeText.Color = YELLOW
│     └─ FALSE → SET PlayerTimeText.Color = GREEN
└─ Branch (BossTime < 60)
   └─ TRUE → SET BossTimeText.Color = ORANGE
      └─ FALSE → SET BossTimeText.Color = RED
```

**To use this, modify GameMode's UpdateUI:**
1. In BP_GameMode_TimeBattle → UpdateUI function
2. After setting text, call UIWidgetRef.UpdateTimerColors(PlayerTime, BossTime)

**Blueprint Nodes:**
1. Create function `UpdateTimerColors`
2. Inputs: PlayerTime (Float), BossTime (Float)
3. `Branch` (PlayerTime < 20)
   - True: `Set Color and Opacity` (PlayerTimeText) = Yellow
   - False: `Set Color and Opacity` (PlayerTimeText) = Green
4. `Branch` (BossTime < 60)
   - True: `Set Color and Opacity` (BossTimeText) = Orange
   - False: `Set Color and Opacity` (BossTimeText) = Red

### 7. Add Visual Effects (Optional)

**Add Background Panel:**
```
Canvas Panel
└─ Overlay
   ├─ Image (Background - semi-transparent black)
   │  └─ Size: 400x150, Color: Black Alpha 0.5
   └─ Vertical Box (Timers as before)
```

**Background Image Setup:**
1. Palette → Common → Image
2. Place in Overlay (add Overlay first)
3. Brush → Color: (0, 0, 0, 0.5) - semi-transparent black
4. Alignment: Center
5. Size: Fixed - 400x150

### 8. Add Animations (Optional Advanced)

**Create Animation for Low Time Warning:**

1. Animations panel → + Animation
2. Name: `LowTimeWarning`
3. Track: PlayerTimeText → Color and Opacity
4. Keyframes:
   - 0.0s: Green
   - 0.5s: Yellow
   - 1.0s: Green
5. Loop: Checked

**Play animation in Graph:**
```
EVENT Tick
├─ GET PlayerTime from GameMode
└─ Branch (PlayerTime < 20 AND not already playing)
   └─ TRUE → Play Animation (LowTimeWarning, Loop)
```

## Testing Checklist
- [ ] UI displays at game start
- [ ] Player timer shows correctly
- [ ] Boss timer shows correctly
- [ ] Timers update when combat happens
- [ ] Text is readable and properly sized
- [ ] Colors differentiate player (green) vs boss (red)

## Quick Reference

**Text Variables:**
- `PlayerTimeText` - Displays player remaining time
- `BossTimeText` - Displays boss remaining time

**Updated from GameMode:**
- GameMode's `UpdateUI()` function sets these text values
- Format: "Player: XXs" and "Boss: XXs"

**Color Scheme:**
- Player: Green (#00FF00) - represents life/hope
- Boss: Red (#FF0000) - represents danger
- Optional: Yellow/Orange for low time warnings

## Troubleshooting

**Text not updating?**
- Ensure variables are marked "Is Variable" ✓
- Check GameMode has valid UIWidgetRef
- Verify UpdateUI is being called

**Text not visible?**
- Check Z-Order (layer ordering)
- Ensure Canvas Panel is root
- Check color alpha is 1.0 (opaque)

**Wrong position?**
- Adjust Anchors
- Check Alignment settings
- Verify Canvas Panel slot settings
