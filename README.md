# Time Master - Boss Fight

A raylib game where time is your health! Fight a boss whose HP is represented by a 5-minute timer while managing your own 1-minute timer.

## Game Mechanics

### Your Timer (Player HP)
- Starts at **1 minute (60 seconds)**
- Decreases by **5 seconds** each time you get hit by a boss projectile
- Can be replenished by collecting **tomatoes** (+5 seconds each)
- Maximum cap: 2 minutes
- Game over when it reaches 0

### Boss Timer (Boss HP)
- Starts at **5 minutes (300 seconds)**
- Decreases by **3 seconds** each time you successfully attack the boss
- Victory when it reaches 0

### Controls
- **Mouse**: Look around (camera control - cursor locked)
- **WASD** or **Arrow Keys**: Move your character (camera-relative movement)
- **SPACE**: Attack the boss (only works when you're close to the boss)
- **ENTER**: Start game / Retry
- **ESC**: Toggle cursor lock (unlock/lock mouse)
- **C**: Toggle camera mode (third-person / static)
- **H**: Toggle boss hitbox debug visualization
- **Mouse Wheel**: Zoom in/out

### Gameplay Tips
- Stay close to the boss to attack, but dodge its projectiles!
- Tomatoes spawn randomly on the ground - collect them to restore your time
- Tomatoes disappear after 8 seconds if not collected
- The boss shoots projectiles at you periodically
- Watch both timers - manage your time wisely!

## Building and Running

### Prerequisites
- C++ compiler (g++)
- raylib library installed

### Install raylib (Ubuntu/Debian)
```bash
sudo apt-get install libraylib-dev
```

### Build and Run
```bash
make
make run
```

Or compile manually:
```bash
g++ -std=c++17 main.cpp -o time_master -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./time_master
```

### Clean
```bash
make clean
```

## Game Design
- **Player**: Blue rectangular character (AABB collision) with camera-relative movement
- **Boss**: Animated Plant Boss (3D GLTF model) with smooth rotation and state machine
  - States: IDLE, ATTACK_1, ATTACK_2, ATTACK_3
  - Smoothly rotates to face player
  - 2.5x player size
- **Tomatoes**: Red circles with green stems that appear randomly
- **Boss Projectiles**: Orange circles that track your position
- **Collision**: 3D rectangular AABB collision system
- **Camera**: Third-person camera with mouse control (FPS-style locked cursor)

Enjoy the game and master the time!
