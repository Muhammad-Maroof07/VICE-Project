# VICE: Project Doom

Doom-style 2D platformer in C++ with SFML 2.6.1 (Programming Fundamentals, Fall 2026).

## Instructions

**Requirements:** Ubuntu, g++, SFML 2.6.1 (does not compile with SFML 3.x).

**Compile and run** from the folder containing `Data/`:

```bash
g++ -c vice_project_doom.cpp
g++ vice_project_doom.o -o vice-project-doom -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
./vice-project-doom
```

**Rules:** no global variables (except `screen_x`, `screen_y`), only allowed SFML functions.

## Structure

```
.
├── vice_project_doom.cpp   (all code)
├── Data/                   (sprites, background, audio)
├── README.md
└── .gitignore
```

| Part | Functions |
|---|---|
| Level | `create_level`, `load_level`, `display_level` |
| Player | `player_input`, `player_gravity`, `player_collision`, `player_animate` |
| Weapons | `switch_weapon`, `fire_whip`, `fire_magnum`, `throw_grenade` |
| Enemies | `spawn_enemies`, `update_enemies`, `draw_enemies` |
| Screens | `check_screen_change` |
| Power-ups | `update_powerups` |
| UI and score | `draw_hud`, `update_score` |

**Each frame:** read input, update player, update enemies, check collisions, check screen change, clear, draw, display.

## Divided Work

| Member | Branch | Responsibility |
|---|---|---|
| Member 1 (Muhammad Maroof) | `player-movement` | Player: movement, jump, duck, ladders, collision, animation |
| Member 2 | `enemies-weapons` | Enemies, weapons, ammo, damage, power-ups |
| Member 3 | `levels-ui` | Level layouts, flip-screen, HUD, scoring, timer |

Each member edits only their own functions to avoid merge conflicts.

## Git Commands

**First time (get the project):**
```bash
git clone <repo-url>
cd <repo-folder>
git checkout -b your-branch-name
```

**Receive updates from teammates (do this before starting work and before pushing):**
```bash
git checkout main
git pull origin main
git checkout your-branch-name
git merge main
```

**Send your updates:**
```bash
git add .
git commit -m "short message about what you did"
git push origin your-branch-name
```

**Merge into main (only when it compiles and runs):**
```bash
git checkout main
git pull origin main
git merge your-branch-name
git push origin main
```

If Git reports a merge conflict, open the marked file, keep the correct lines, then run `git add .` and `git commit`.
