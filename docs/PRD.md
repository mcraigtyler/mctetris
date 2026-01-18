# PRD: CLI Tetris (C++/ncurses)

## Overview
Build a classic Tetris clone that runs in a Linux terminal using box-drawing and block characters with keyboard input. The game should provide a simple menu, classic gameplay rules, and a high score list. It must compile with CMake and run in the default Linux Mint terminal (GNOME Terminal).

## Goals
- Playable, responsive Tetris in a terminal.
- Simple menu: Start, High Scores, Quit.
- Classic rules with next-piece preview, soft drop, and hard drop.
- Persist and display high scores.
- Easy to build/debug with CMake and GCC.

## Non-Goals
- Multiplayer.
- Advanced rule variants (hold piece, T-spins, combos).
- Fancy graphics or audio.
- Online leaderboards.

## Target Platform & Tooling
- OS: Linux (target terminal: GNOME Terminal).
- Language: C++ (C++17 or newer).
- UI/Input: `ncurses`.
- Build system: CMake (3.28+ available).
- Compiler: GCC (13.3+ available).
- Terminal requirements: UTF-8 locale and a font that supports box-drawing characters.

## User Experience
### Menu
- Keyboard navigation (up/down + enter).
- Options: Start, Control Scheme, High Scores, Quit.
- Clear prompts and consistent text layout.

### Gameplay
- 10x20 playfield with border.
- Next-piece preview panel.
- Score, level, and lines cleared displayed.
- Pausable gameplay.
- Pieces render as background-colored cells (space characters with color pairs). Empty cells render as blank spaces.

## Gameplay Rules
- 7 standard tetrominoes.
- Gravity increases with level.
- Lock when piece lands and cannot move down.
- Line clears remove full rows and shift above rows down.
- Game ends when a new piece cannot spawn.

## Controls
### Control Schemes
Control schemes are designed to be configurable in a future phase.
Default scheme: WASD.

#### Arrows
- Left/Right: move.
- Up: rotate.
- Down: soft drop.
- Space: hard drop.

#### NumPad
- 4/6: move.
- 7/9: rotate.
- 8: soft drop.
- 5: hard drop.

#### WASD
- A/D: move.
- Q/E: rotate.
- W: soft drop.
- S: hard drop.

### Global Controls (all schemes)
- P: pause.
- Q: quit to menu.

## Scoring & Progression (Classic)
- Single: 40 × (level + 1).
- Double: 100 × (level + 1).
- Triple: 300 × (level + 1).
- Tetris: 1200 × (level + 1).
- Level increases every 10 lines.

## Visuals & Colors
- Each tetromino uses a distinct color.
- Colors are applied via ncurses color pairs on background cells.
- The color palette is designed to be swappable via a future menu option.

## High Scores
- Store top N scores in a local file under the user's home directory.
- Display name (or initials), score, and level.
- File location: user home (e.g., `~/.mctetris_scores`).

## Functional Requirements
- Non-blocking input loop.
- Smooth refresh without flicker.
- Deterministic rotation and collision.
- Safe file I/O for score persistence.
- Control scheme selection stored in memory for the session.
- Control mappings are defined in a way that can be externalized later.

## Acceptance Criteria
- `cmake` build produces a runnable binary.
- Menu works and is navigable via keyboard.
- Gameplay includes next-piece preview, soft drop, hard drop.
- Classic scoring and level progression are implemented.
- High scores persist across runs.
- Runs on GNOME Terminal.

## Open Questions
- Final score file location: project-local vs. user home.
- Default key bindings if terminal does not support arrows well.
