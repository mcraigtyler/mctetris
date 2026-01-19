#pragma once

#include "point.h"
#include "tetromino.h"

namespace mctetris::model {

struct ActivePiece {
    Tetromino piece{};
    Point origin{0, 0};
};

} // namespace mctetris::model
