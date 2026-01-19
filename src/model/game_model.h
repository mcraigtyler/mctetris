#pragma once

#include <optional>

#include "active_piece.h"
#include "board.h"

namespace mctetris::model {

class GameModel {
  public:
    [[nodiscard]] bool Spawn(TetrominoType type);
    [[nodiscard]] bool Move(int dx, int dy);
    [[nodiscard]] bool RotateCW();
    [[nodiscard]] bool SoftDrop();
    void HardDrop();
    void TickGravity();
    [[nodiscard]] bool IsGameOver() const;
    [[nodiscard]] int Level() const;
    [[nodiscard]] int LinesCleared() const;
    [[nodiscard]] int Score() const;
    [[nodiscard]] int GravityDelayMs() const;
    [[nodiscard]] const Board &GetBoard() const;
    [[nodiscard]] const std::optional<ActivePiece> &CurrentPiece() const;

  private:
    [[nodiscard]] bool CanPlaceAt(const Tetromino &piece, const Point &origin) const;
    void LockPiece();
    void UpdateLevel();

    Board board_{};
    std::optional<ActivePiece> current_{};
    int linesCleared_ = 0;
    int level_ = 0;
    int score_ = 0;
    bool gameOver_ = false;
};

} // namespace mctetris::model
