#pragma once

#include <array>
#include <cstdint>

#include "point.h"

namespace mctetris::model {

enum class Cell : std::uint8_t {
    Empty = 0,
    I,
    O,
    T,
    S,
    Z,
    J,
    L
};

enum class TetrominoType : std::uint8_t {
    I,
    O,
    T,
    S,
    Z,
    J,
    L
};

struct Tetromino {
    TetrominoType type = TetrominoType::I;
    int rotation = 0;

    [[nodiscard]] std::array<Point, 4> Blocks() const;
    [[nodiscard]] Cell CellType() const;
};

}  // namespace mctetris::model
