#pragma once

#include <optional>

#include "active_piece.h"
#include "board.h"

namespace mctetris::model {

class GameModel {
  public:
    [[nodiscard]] bool Spawn(TetrominoType type);
    [[nodiscard]] const Board &GetBoard() const;
    [[nodiscard]] const std::optional<ActivePiece> &CurrentPiece() const;

  private:
    Board board_{};
    std::optional<ActivePiece> current_{};
};

}  // namespace mctetris::model
