# AI Agent Instructions

This file describes how AI agents should work in this repository.

## Project Context
- CLI Tetris clone for Linux terminals built in C++17 with ncurses.
- Primary build system: CMake.

## Documentation and Instructions
- Project docs: `docs/PRD.md`, `docs/Screen-Mockups.md`, `docs/Plan.md`.
- Instruction files for code standards: `.github/instructions/*.instructions.md`.
- C++ style guide: `.github/instructions/cxx.instructions.md`.
- Markdown guidelines: `.github/instructions/markdown.instructions.md`.

## Required Verification
When making code changes, verify the following steps when feasible:
```bash
cmake -S . -B build
cmake --build build
cmake --build build --target format
```

If you cannot run the commands, state that and explain why.
