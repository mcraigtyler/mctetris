#include "tetromino.h"

namespace mctetris::model {
namespace {

constexpr std::array<std::array<Point, 4>, 4> kI = {{
    std::array<Point, 4>{Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{3, 1}},
    std::array<Point, 4>{Point{2, 0}, Point{2, 1}, Point{2, 2}, Point{2, 3}},
    std::array<Point, 4>{Point{0, 2}, Point{1, 2}, Point{2, 2}, Point{3, 2}},
    std::array<Point, 4>{Point{1, 0}, Point{1, 1}, Point{1, 2}, Point{1, 3}},
}};

constexpr std::array<std::array<Point, 4>, 4> kO = {{
    std::array<Point, 4>{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{2, 1}},
    std::array<Point, 4>{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{2, 1}},
    std::array<Point, 4>{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{2, 1}},
    std::array<Point, 4>{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{2, 1}},
}};

constexpr std::array<std::array<Point, 4>, 4> kT = {{
    std::array<Point, 4>{Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1}},
    std::array<Point, 4>{Point{1, 0}, Point{1, 1}, Point{2, 1}, Point{1, 2}},
    std::array<Point, 4>{Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{1, 2}},
    std::array<Point, 4>{Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{1, 2}},
}};

constexpr std::array<std::array<Point, 4>, 4> kS = {{
    std::array<Point, 4>{Point{1, 0}, Point{2, 0}, Point{0, 1}, Point{1, 1}},
    std::array<Point, 4>{Point{1, 0}, Point{1, 1}, Point{2, 1}, Point{2, 2}},
    std::array<Point, 4>{Point{1, 1}, Point{2, 1}, Point{0, 2}, Point{1, 2}},
    std::array<Point, 4>{Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{1, 2}},
}};

constexpr std::array<std::array<Point, 4>, 4> kZ = {{
    std::array<Point, 4>{Point{0, 0}, Point{1, 0}, Point{1, 1}, Point{2, 1}},
    std::array<Point, 4>{Point{2, 0}, Point{1, 1}, Point{2, 1}, Point{1, 2}},
    std::array<Point, 4>{Point{0, 1}, Point{1, 1}, Point{1, 2}, Point{2, 2}},
    std::array<Point, 4>{Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{0, 2}},
}};

constexpr std::array<std::array<Point, 4>, 4> kJ = {{
    std::array<Point, 4>{Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1}},
    std::array<Point, 4>{Point{1, 0}, Point{2, 0}, Point{1, 1}, Point{1, 2}},
    std::array<Point, 4>{Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{2, 2}},
    std::array<Point, 4>{Point{1, 0}, Point{1, 1}, Point{0, 2}, Point{1, 2}},
}};

constexpr std::array<std::array<Point, 4>, 4> kL = {{
    std::array<Point, 4>{Point{2, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1}},
    std::array<Point, 4>{Point{1, 0}, Point{1, 1}, Point{1, 2}, Point{2, 2}},
    std::array<Point, 4>{Point{0, 1}, Point{1, 1}, Point{2, 1}, Point{0, 2}},
    std::array<Point, 4>{Point{0, 0}, Point{1, 0}, Point{1, 1}, Point{1, 2}},
}};

constexpr const std::array<std::array<Point, 4>, 4> &ShapeFor(TetrominoType type) {
    switch (type) {
    case TetrominoType::I:
        return kI;
    case TetrominoType::O:
        return kO;
    case TetrominoType::T:
        return kT;
    case TetrominoType::S:
        return kS;
    case TetrominoType::Z:
        return kZ;
    case TetrominoType::J:
        return kJ;
    case TetrominoType::L:
        return kL;
    }
    return kI;
}

} // namespace

std::array<Point, 4> Tetromino::Blocks() const {
    const auto &shape = ShapeFor(type);
    return shape[rotation % 4];
}

Cell Tetromino::CellType() const {
    switch (type) {
    case TetrominoType::I:
        return Cell::I;
    case TetrominoType::O:
        return Cell::O;
    case TetrominoType::T:
        return Cell::T;
    case TetrominoType::S:
        return Cell::S;
    case TetrominoType::Z:
        return Cell::Z;
    case TetrominoType::J:
        return Cell::J;
    case TetrominoType::L:
        return Cell::L;
    }
    return Cell::Empty;
}

} // namespace mctetris::model
