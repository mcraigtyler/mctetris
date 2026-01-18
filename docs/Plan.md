# Implementation Plan (Phased)

## Phase 1: Build & Project Scaffolding
- Create CMake project structure and targets.
- Add ncurses detection/linking.
- Provide a minimal `main` that initializes/tears down ncurses.
- Add basic build instructions in README (if/when present).

## Phase 2: Core Game Model
- Implement board representation (10x20) and cell types.
- Define tetromino shapes and rotation states.
- Implement collision detection and spawn logic.
- Add line clear detection and board compaction.

## Phase 3: Game Loop & Timing
- Add main game loop with tick timing (gravity).
- Implement piece movement, rotation, soft drop, hard drop.
- Implement lock delay or immediate lock on collision (classic).
- Add level progression and gravity scaling.

## Phase 4: Rendering & HUD
- Render borders using box-drawing characters.
- Render playfield using colored background cells.
- Render next-piece panel and stats (score/level/lines).
- Add pause overlay and game over state.

## Phase 5: Menu & Control Schemes
- Implement main menu with keyboard navigation.
- Add control scheme menu (WASD default, Arrows, NumPad).
- Store selected control scheme in session state.

## Phase 6: High Scores
- Define score record format (name/score/level).
- Implement load/save in user home (e.g., `~/.mctetris_scores`).
- Display high score screen.

## Phase 7: Polish & Testing
- Playtest for input responsiveness and timing.
- Verify rendering alignment in GNOME Terminal.
- Verify score persistence and edge cases (top-out, line clears).
- Fix bugs and adjust balance.
