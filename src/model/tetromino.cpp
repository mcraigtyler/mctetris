#include "tetromino.h"

namespace mctetris::model {
namespace {

constexpr std::array<std::array<Point, 4>, 4> kI = {{{{0, 1}, {1, 1}, {2, 1}, {3, 1}},
                                                    {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
                                                    {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
                                                    {{1, 0}, {1, 1}, {1, 2}, {1, 3}}}};

constexpr std::array<std::array<Point, 4>, 4> kO = {{{{1, 0}, {2, 0}, {1, 1}, {2, 1}},
                                                    {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
                                                    {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
                                                    {{1, 0}, {2, 0}, {1, 1}, {2, 1}}}};

constexpr std::array<std::array<Point, 4>, 4> kT = {{{{1, 0}, {0, 1}, {1, 1}, {2, 1}},
                                                    {{1, 0}, {1, 1}, {2, 1}, {1, 2}},
                                                    {{0, 1}, {1, 1}, {2, 1}, {1, 2}},
                                                    {{1, 0}, {0, 1}, {1, 1}, {1, 2}}}};

constexpr std::array<std::array<Point, 4>, 4> kS = {{{{1, 0}, {2, 0}, {0, 1}, {1, 1}},
                                                    {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
                                                    {{1, 1}, {2, 1}, {0, 2}, {1, 2}},
                                                    {{0, 0}, {0, 1}, {1, 1}, {1, 2}}}};

constexpr std::array<std::array<Point, 4>, 4> kZ = {{{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
                                                    {{2, 0}, {1, 1}, {2, 1}, {1, 2}},
                                                    {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
                                                    {{1, 0}, {0, 1}, {1, 1}, {0, 2}}}};

constexpr std::array<std::array<Point, 4>, 4> kJ = {{{{0, 0}, {0, 1}, {1, 1}, {2, 1}},
                                                    {{1, 0}, {2, 0}, {1, 1}, {1, 2}},
                                                    {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
                                                    {{1, 0}, {1, 1}, {0, 2}, {1, 2}}}};

constexpr std::array<std::array<Point, 4>, 4> kL = {{{{2, 0}, {0, 1}, {1, 1}, {2, 1}},
                                                    {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
                                                    {{0, 1}, {1, 1}, {2, 1}, {0, 2}},
                                                    {{0, 0}, {1, 0}, {1, 1}, {1, 2}}}};

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

}  // namespace

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

}  // namespace mctetris::model
