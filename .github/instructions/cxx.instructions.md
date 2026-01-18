---
description: 'C++17 coding standards and best practices for this repository'
applyTo: '**/*.h, **/*.hpp, **/*.cpp, **/*.cxx, **/*.cc'
---

# C++ Instructions

Guidelines for writing C++17 code in this repository. Follow these rules for new
or modified C++ files unless a file already uses a different established
pattern.

## General Instructions

- Prefer clear, simple solutions over clever ones.
- Keep functions small and focused; extract helpers when logic branches grow.
- Use const-correctness and pass by const reference where appropriate.
- Avoid macros except for include guards and compile-time flags.
- Use standard library facilities instead of custom utilities when possible.

## Best Practices

- Use RAII for resource management; avoid manual `new`/`delete`.
- Favor value semantics; use move semantics when ownership transfers.
- Validate inputs at boundaries (public APIs, file IO, user input).
- Avoid global mutable state; prefer localized state and dependency injection.
- Keep error paths explicit and testable.

## Code Standards

### Naming

- Types: `PascalCase` (e.g., `GameModel`).
- Functions and variables: `camelCase` (e.g., `spawnTetromino`).
- Constants: `kPascalCase` for `constexpr` globals (e.g., `kBoardWidth`).
- Files: `snake_case.cpp/.h` when adding new files, unless matching an existing
  convention.

### Formatting

- Indent with 4 spaces. No tabs.
- One statement per line.
- Braces: K&R style.
- Limit line length to 100 columns when practical.
- Keep includes ordered: standard library, third-party, then project headers.

## Architecture and Structure

- Keep headers self-contained and compile on their own.
- Prefer forward declarations in headers when possible.
- Minimize inline implementation in headers; favor `.cpp` files.
- Avoid circular dependencies; move shared types to common headers if needed.

## Error Handling

- Prefer returning `std::optional` or error enums for expected failures.
- Use exceptions only for truly exceptional cases and be consistent within a
  subsystem.
- Avoid sentinel values when a type can express absence or failure.

## Performance

- Avoid premature optimization.
- Reserve capacity for containers when the size is known.
- Avoid unnecessary heap allocations in hot paths.
- Keep data layouts cache-friendly for tight loops.

## Testing

- Add tests for new behavior and bug fixes when feasible.
- Prefer deterministic tests; avoid real-time sleeps.
- Test error paths and boundary conditions.

## Examples

### Good Example
```cpp
std::optional<int> parseIndex(const std::string& text) {
    if (text.empty()) {
        return std::nullopt;
    }
    return std::stoi(text);
}
```

### Bad Example
```cpp
int parseIndex(const char* text) {
    if (!text || !*text) {
        return -1;
    }
    return atoi(text);
}
```

## Validation and Verification

- Build: `cmake -S . -B build && cmake --build build`
- Run: `./build/mctetris`
