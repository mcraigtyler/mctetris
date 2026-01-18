# AI Agent Instructions

This file describes how AI agents should work in this repository.

## Project Context
- CLI Tetris clone for Linux terminals built in C++17 with ncurses.
- Primary build system: CMake.
- vcpkg is included as a submodule in `external/vcpkg` for dependency management.

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

If system ncurses is unavailable, use the vcpkg preset after bootstrapping:
```bash
git submodule update --init --recursive
./external/vcpkg/bootstrap-vcpkg.sh
cmake --preset vcpkg
cmake --build build
```

If you cannot run the commands, state that and explain why.
