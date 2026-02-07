# 🎮 Third-Person Camera - Guide d'utilisation

## ✅ IMPLÉMENTATION TERMINÉE !

Votre jeu dispose maintenant d'une **caméra third-person** complète qui suit le joueur !

---

## 🎯 Fonctionnalités

### Caméra Third-Person Automatique
- ✅ La caméra suit le joueur automatiquement
- ✅ Position derrière et au-dessus du joueur
- ✅ Vue smooth et fluide

### Contrôles de Caméra

| Touche/Action | Fonction |
|---------------|----------|
| **Clic droit souris + Mouvement** | Rotation de la caméra autour du joueur |
| **Molette souris** | Zoom avant/arrière (100-800 unités) |
| **C** | Basculer entre vue third-person et vue statique |

### Paramètres

| Paramètre | Valeur | Description |
|-----------|--------|-------------|
| Distance initiale | 400 unités | Distance de la caméra au joueur |
| Hauteur | 300 unités | Hauteur au-dessus du joueur |
| FOV | 45° | Champ de vision |
| Sensibilité souris | 0.1 | Vitesse de rotation |

---

## 🎮 Comment jouer avec la nouvelle caméra

### Démarrage
```bash
make run
# ou
./time_master
```

### En jeu :

1. **Mouvement** : WASD ou flèches directionnelles
2. **Rotation caméra** : Maintenir clic droit + bouger la souris
3. **Zoom** : Molette de la souris
4. **Changer de vue** : Touche C (third-person ↔ statique)
5. **Attaque** : Espace (s'approcher du boss)

---

## 🔧 Comment ça fonctionne

### Architecture

```
CameraManager
├─ m_distance       → Distance du joueur
├─ m_height         → Hauteur au-dessus du joueur  
├─ m_angleAroundPlayer → Rotation horizontale (0-360°)
├─ m_pitch          → Angle vertical (5-80°)
└─ m_isThirdPerson  → Mode actif/statique
```

### Calcul de position

```cpp
// Position caméra calculée chaque frame:
horizontalDistance = distance * cos(pitch)
verticalDistance = distance * sin(pitch)

offsetX = horizontalDistance * sin(angleAroundPlayer)
offsetZ = horizontalDistance * cos(angleAroundPlayer)

camera.position = playerPosition - offset
camera.target = playerPosition + (0, 30, 0)
```

---

## 🎨 Personnalisation

### Changer la distance par défaut

**Dans `include/Config.hpp`** :
```cpp
constexpr float CAMERA_DISTANCE = 400.0f;  // Changez cette valeur
```

### Changer la sensibilité de la souris

**Dans `src/CameraManager.cpp`** ligne ~13 :
```cpp
m_mouseSensitivity(0.1f)  // Augmentez pour rotation plus rapide
```

### Changer les limites de zoom

**Dans `src/CameraManager.cpp`** fonction `AdjustDistance()` :
```cpp
if (m_distance < 100.0f) m_distance = 100.0f;  // Zoom min
if (m_distance > 800.0f) m_distance = 800.0f;  // Zoom max
```

### Changer l'angle de vue initial

**Dans `src/CameraManager.cpp`** constructeur :
```cpp
m_pitch(20.0f)  // Angle vertical initial (en degrés)
m_angleAroundPlayer(0.0f)  // Angle horizontal initial
```

---

## 🚀 Améliorations possibles

### Option 1 : Collision de caméra
Empêcher la caméra de traverser les murs/obstacles.

### Option 2 : Caméra dynamique
Ajuster automatiquement la distance pendant le combat.

### Option 3 : Camera shake
Effet de secousse lors des impacts.

### Option 4 : Smooth transitions
Interpolation douce lors du changement de mode.

### Option 5 : Look-at offset
Viser légèrement devant le joueur dans la direction du mouvement.

---

## 🐛 Troubleshooting

### La caméra ne suit pas le joueur
- Vérifier que vous êtes en mode third-person (appuyer sur C)
- Le mode par défaut est third-person

### La rotation est trop rapide/lente
- Ajuster `m_mouseSensitivity` dans le constructeur

### Le zoom ne fonctionne pas
- Vérifier que votre souris a une molette fonctionnelle
- Tester avec les touches +/- si besoin

### La caméra traverse le sol
- Normal pour l'instant, il faudrait implémenter une collision de caméra

---

## 📝 Code modifié

### Fichiers modifiés :
1. ✅ `include/CameraManager.hpp` - Nouveaux membres et méthodes
2. ✅ `src/CameraManager.cpp` - Logique third-person complète
3. ✅ `src/Game.cpp` - Appel UpdateThirdPerson() dans la boucle

### Aucun fichier supplémentaire nécessaire !

---

## 🎯 Résumé

Vous avez maintenant :
- ✅ Caméra qui suit le joueur en third-person
- ✅ Rotation avec clic droit + souris
- ✅ Zoom avec molette
- ✅ Basculement entre vues avec touche C
- ✅ Angles limités pour éviter les bugs
- ✅ Code propre et facilement personnalisable

**Le jeu est maintenant beaucoup plus immersif ! 🎮**

---

**Prochaine étape suggérée :**
Voulez-vous qu'on ajoute un système de tir avec projectiles pour le joueur ? 🚀
