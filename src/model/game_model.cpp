#include "game_model.h"

namespace mctetris::model {

bool GameModel::Spawn(TetrominoType type) {
    Tetromino piece{type, 0};
    const int spawnX = 3;
    const int spawnY = 0;
    if (!board_.CanPlace(piece, spawnX, spawnY)) {
        current_.reset();
        return false;
    }
    current_ = ActivePiece{piece, {spawnX, spawnY}};
    return true;
}

const Board &GameModel::GetBoard() const {
    return board_;
}

const std::optional<ActivePiece> &GameModel::CurrentPiece() const {
    return current_;
}

}  // namespace mctetris::model
