# Maps / Levels

## À créer dans Unreal Engine :

### Arena - Level Principal
**Type:** New Level → Empty Level ou Basic

---

## Création de la Map Arena :

### 1. Créer le niveau
1. File → New Level → Empty Level
2. Sauvegarder: Content/Maps/Arena

### 2. Lighting de base
**Ajouter ces actors:**
- **Sky Light**
  - Intensity: 1.0
  - Light Color: White
  
- **Directional Light** (Sun)
  - Intensity: 5.0
  - Rotation: -45° pitch pour ombres
  
- **Sky Atmosphere**
  - Pour le ciel réaliste

- **Volumetric Cloud** (optionnel)
  - Pour l'ambiance

### 3. Arène de combat

**Floor (Sol):**
- Cube Static Mesh ou Plane
- Scale: X=50, Y=50, Z=1
- Material: M_PrototypeGrid (du template)
- Position: Z=0

**Walls (Murs) - optionnels:**
- 4 cubes pour former un carré
- Hauteur: Z=5
- Collision: BlockAll

**Player Start:**
- Place Actor → Player Start
- Position: X=0, Y=0, Z=100
- Rotation: Regarder vers le centre

**Boss Spawn Point:**
- Placer BP_BossCharacter
- Position: X=1000, Y=0, Z=100 (1000 unités du player)
- Le boss va automatiquement tracking le joueur

### 4. Dimensions recommandées

**Arène circulaire (simple):**
- Rayon: 2000 units
- Floor: Cylinder ou grand carré

**Arène carrée:**
- Taille: 4000 x 4000 units
- Murs à 2000 units du centre

### 5. Props / Décoration (optionnel)

**Obstacles:**
- Quelques cubes (200x200x200)
- Positionnés aléatoirement
- Collision: BlockAll
- Pour stratégie de combat

**Lighting d'ambiance:**
- Point Lights aux coins
- Intensity: 2000
- Color: légèrement bleutée
- Radius: 1000

### 6. Post-Process Volume

**Ajouter un Post Process Volume:**
- Unbound: ✓ (coché)
- Ambient Occlusion: Enabled
- Bloom: Intensity 0.5
- Auto Exposure: Min/Max = 1.0 (pour stabilité)

### 7. Configuration Level

**World Settings (Window → World Settings):**
- Game Mode Override: BP_TimeBossGameMode
- Kill Z: -1000 (reset si tombent)

### 8. Navigation Mesh (pour Boss AI)

**Si vous voulez que le boss évite les obstacles:**
1. Modes → Volumes → Nav Mesh Bounds Volume
2. Scale pour couvrir toute l'arène
3. Press 'P' en jeu pour voir le navmesh (vert)

---

## Layout Recommandé (Vue du dessus)

```
                    N
                    ↑
    +---------------------------+
    |                           |
    |  [Boss]                   |
    |    ●                      |
    |         [Obstacle]        |
    |            ▪              |
    |                           |
W ← |         [Center]          | → E
    |            +              |
    |                           |
    |         [Obstacle]        |
    |            ▪              |
    |                     ●     |
    |                  [Player] |
    |                           |
    +---------------------------+
                    ↓
                    S

Distance Player-Boss: ~1000-1500 units
Taille arène: 4000x4000 units
```

---

## Quick Test Checklist

- [ ] Player Start présent et positionné
- [ ] BP_BossCharacter placé dans le level
- [ ] Floor collision fonctionne
- [ ] Lighting visible (pas tout noir)
- [ ] Game Mode configuré (World Settings)
- [ ] Les deux timers apparaissent en haut
- [ ] Player peut bouger (WASD)
- [ ] Boss se déplace vers le player
- [ ] Attaque fonctionne (Left Click)

---

## Variantes de Maps (Game Jam ideas)

### 1. Simple Arena
- Sol plat
- Murs circulaires
- Pas d'obstacles
- Focus: Combat pur

### 2. Obstacle Course
- Plusieurs piliers
- Cover strategy
- Boss doit contourner

### 3. Multi-Level
- Plateformes à différentes hauteurs
- Rampes
- Verticalité

### 4. Shrinking Arena
- Murs qui se rapprochent avec le temps
- Urgence supplémentaire

---

## Performance Tips

**Pour Game Jam (performance > graphics):**
- Static Lighting (Build → Build Lighting)
- Lightmass Importance Volume
- Pas trop de dynamic lights
- LOD sur les meshes
- Occlusion Culling activé

**Commandes Console (~ key):**
- `stat fps` - Voir les FPS
- `stat unit` - Performance breakdown
- `r.ScreenPercentage 50` - Baisser résolution
- `t.MaxFPS 60` - Cap FPS
