# Time Master - Boss Fight

A raylib game where time is your health! Fight a boss whose HP is represented by a 5-minute timer while managing your own 1-minute timer.

## Game Mechanics

### Your Timer (Player HP)
- Starts at **1 minute (60 seconds)**
- Decreases by **5 seconds** each time you get hit by a boss projectile
- Can be replenished by collecting **tomatoes** (+5 seconds each)
- Maximum cap: 2 minutes
- **Dashing without stamina costs 5 seconds of time!**
- Game over when it reaches 0

### Stamina System
- Maximum stamina: **100 points**
- Each dash costs **25 stamina**
- Regenerates at **10 stamina per second** (takes 10 seconds to fully recharge)
- **Dash duration**: 0.5 seconds with +200 speed boost
- **Dash cooldown**: 0.5 seconds between dashes
- **Warning**: If you dash with 0 stamina, it will cost **5 seconds of your time** instead!
- Stamina bar displayed in the HUD (green/yellow/red based on amount)

### Boss Timer (Boss HP)
- Starts at **5 minutes (300 seconds)**
- Decreases by **3 seconds** each time you successfully attack the boss
- Victory when it reaches 0

### Controls
- **Mouse**: Look around (camera control - cursor locked)
- **Left Mouse Button**: Shoot projectiles at the boss (fast attack - 0.2s cooldown)
- **WASD** or **Arrow Keys**: Move your character (camera-relative movement)
- **SHIFT**: Dash in the movement direction (costs 25 stamina, or 5 seconds if out of stamina)
- **SPACE**: Melee attack the boss (only works when you're close to the boss)
- **ENTER**: Start game / Retry
- **ESC**: Toggle cursor lock (unlock/lock mouse)
- **C**: Toggle camera mode (third-person / static)
- **H**: Toggle boss hitbox debug visualization
- **Mouse Wheel**: Zoom in/out

### Gameplay Tips
- Use your fast projectile attacks to damage the boss from a distance!
- **Use SHIFT to dash** - 0.5 second burst with **+200 speed** for dramatic escapes
- **Base movement speed**: 75 units/second | **Dash speed**: 275 units/second (nearly 4x faster!)
- **Manage your stamina carefully!** Each dash costs 25 stamina with a 0.5s cooldown
- **Stamina regenerates slowly** at 10/second - it takes 10 seconds to fully recharge
- You can only dash 4 times before depleting stamina - use wisely!
- **NEVER dash with 0 stamina** - it will cost you 5 seconds of precious time!
- Watch the stamina bar color: green = safe, yellow = low, red = critical
- Dash creates a cyan particle trail effect for visual feedback
- Stay close to the boss for melee attacks, but dodge its projectiles!
<<<<<<< HEAD
- **The boss attacks every 3 seconds** - use the idle window to position yourself
- **Watch out for boss AoE attacks** - they trigger at 3/4 of the attack animation
- Watch the boss animation carefully to time your dodges for AoE attacks
=======
- **Watch out for boss AoE attacks** - keep your distance during ATTACK_2 and ATTACK_3!
>>>>>>> c84064823257e97429fae742b52f04f09bea2961
- ATTACK_2 has a 150-unit range, ATTACK_3 has a 200-unit range
- Tomatoes spawn randomly on the ground - collect them to restore your time
- Tomatoes disappear after 8 seconds if not collected
- The boss alternates between projectile attacks and area attacks
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
  - Positioned above ground level (y=5) to account for arena visual thickness
  - Two attack modes: melee (SPACE) and ranged projectiles (Left Click)
- **Boss**: Animated Plant Boss (3D GLTF model) with smooth rotation and state machine
  - States: IDLE, ATTACK_1, ATTACK_2, ATTACK_3
<<<<<<< HEAD
  - **Attacks every 3 seconds** (1.5s idle + 1.5s attack)
  - Attack patterns:
    - ATTACK_1: Shoots a projectile at the player (triggers at 0.3s)
    - ATTACK_2: Area of Effect attack (150 unit range, triggers at 3/4 of animation ~1.1s)
    - ATTACK_3: Larger Area of Effect attack (200 unit range, triggers at 3/4 of animation ~1.1s)
  - AoE attacks sync with animation for better visual feedback
=======
  - Attack patterns:
    - ATTACK_1: Shoots a projectile at the player
    - ATTACK_2: Area of Effect attack (150 unit range)
    - ATTACK_3: Larger Area of Effect attack (200 unit range)
>>>>>>> c84064823257e97429fae742b52f04f09bea2961
  - Smoothly rotates to face player
  - Compact hitbox (0.8x base size) matching visual scale
  - Positioned above ground level (y=5) to account for arena visual thickness
- **Camera**: Adjusted closer view (200 distance, 150 height) for better perspective
- **Tomatoes**: Red circles with green stems that appear randomly
- **Boss Projectiles**: Orange circles that track your position (only from ATTACK_1)
- **Collision**: 3D rectangular AABB collision system
- **Particle Effects**: 
  - Brown dust particles for footsteps when moving
  - Cyan/blue trail particles when dashing (enhanced visual feedback)
  - Orange/yellow muzzle flash when shooting projectiles
  - White/silver slash particles for melee attacks
  - Red danger particles for boss AoE attacks
  - All particles have gravity and fade over time
  - Up to 1000 particles can be active simultaneously
- **Camera**: Third-person camera with mouse control (FPS-style locked cursor)

Enjoy the game and master the time!
