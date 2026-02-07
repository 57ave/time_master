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
- **WASD** or **Arrow Keys**: Move your character
- **SPACE**: Attack the boss (only works when you're close to the boss)
- **ENTER**: Start game / Retry
- **ESC**: Return to menu (from game over/victory screen)

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
- **Player**: Blue circle that moves around the arena
- **Boss**: Red rectangle with glowing eyes that moves up and down
- **Tomatoes**: Red circles with green stems that appear randomly
- **Boss Projectiles**: Orange circles that track your position

Enjoy the game and master the time!
