#include <algorithm>

#include "board.h"

namespace mctetris::model {

Board::Board() {
    for (auto &row : cells_) {
        row.fill(Cell::Empty);
    }
}

bool Board::IsInside(int x, int y) const {
    return x >= 0 && x < kBoardWidth && y >= 0 && y < kBoardHeight;
}

bool Board::IsEmpty(int x, int y) const {
    if (!IsInside(x, y)) {
        return false;
    }
    return cells_[y][x] == Cell::Empty;
}

bool Board::CanPlace(const Tetromino &piece, int originX, int originY) const {
    for (const auto &block : piece.Blocks()) {
        const int x = originX + block.x;
        const int y = originY + block.y;
        if (!IsInside(x, y) || !IsEmpty(x, y)) {
            return false;
        }
    }
    return true;
}

void Board::Place(const Tetromino &piece, int originX, int originY) {
    const Cell cell = piece.CellType();
    for (const auto &block : piece.Blocks()) {
        const int x = originX + block.x;
        const int y = originY + block.y;
        if (IsInside(x, y)) {
            cells_[y][x] = cell;
        }
    }
}

int Board::ClearFullLines() {
    int cleared = 0;
    int writeRow = kBoardHeight - 1;

    for (int readRow = kBoardHeight - 1; readRow >= 0; --readRow) {
        const auto &row = cells_[readRow];
        const bool full = std::all_of(row.begin(), row.end(), [](Cell cell) { return cell != Cell::Empty; });
        if (full) {
            ++cleared;
            continue;
        }
        if (writeRow != readRow) {
            cells_[writeRow] = row;
        }
        --writeRow;
    }

    for (int row = writeRow; row >= 0; --row) {
        cells_[row].fill(Cell::Empty);
    }

    return cleared;
}

const std::array<std::array<Cell, kBoardWidth>, kBoardHeight> &Board::Cells() const {
    return cells_;
}

} // namespace mctetris::model
