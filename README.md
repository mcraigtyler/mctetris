# MCTetris

CLI Tetris clone for Linux terminals, built in C++ with ncurses.

## Requirements
- Linux terminal (GNOME Terminal target).
- UTF-8 locale and a font that supports box-drawing characters.
- C++ compiler (GCC 13+ recommended).
- CMake 3.28+.
- ncurses development package (e.g., `libncurses-dev`).

## Install ncurses (Linux)
Debian/Ubuntu:
```bash
sudo apt-get update
sudo apt-get install -y libncurses-dev
```

Fedora:
```bash
sudo dnf install -y ncurses-devel
```

Arch:
```bash
sudo pacman -S --needed ncurses
```

## Build
```bash
cmake -S . -B build
cmake --build build
```

## Run
```bash
./build/mctetris
```

## Controls
Default control scheme: WASD.

- Move: A/D
- Rotate: Q/E
- Soft drop: W
- Hard drop: S
- Pause: P
- Quit to menu: Q

Other schemes (selectable in menu): Arrows, NumPad.

## High Scores
Scores are stored in the user home directory (default: `~/.mctetris_scores`).

## Docs
- `docs/PRD.md`
- `docs/screen-mockup.md`
- `Plan.md`
