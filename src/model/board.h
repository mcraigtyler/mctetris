#pragma once

#include <array>

#include "tetromino.h"

namespace mctetris::model {

constexpr int kBoardWidth = 10;
constexpr int kBoardHeight = 20;

class Board {
  public:
    Board();

    [[nodiscard]] bool IsInside(int x, int y) const;
    [[nodiscard]] bool IsEmpty(int x, int y) const;
    [[nodiscard]] bool CanPlace(const Tetromino &piece, int originX, int originY) const;

    void Place(const Tetromino &piece, int originX, int originY);
    int ClearFullLines();

    [[nodiscard]] const std::array<std::array<Cell, kBoardWidth>, kBoardHeight> &Cells() const;

  private:
    std::array<std::array<Cell, kBoardWidth>, kBoardHeight> cells_{};
};

}  // namespace mctetris::model
