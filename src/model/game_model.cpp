#include "game_model.h"

namespace mctetris::model {
namespace {

constexpr int kSpawnX = 3;
constexpr int kSpawnY = 0;
constexpr int kLinesPerLevel = 10;
constexpr int kBaseGravityMs = 1000;
constexpr int kGravityStepMs = 75;
constexpr int kMinGravityMs = 100;
constexpr int kSingleLineScore = 100;
constexpr int kDoubleLineScore = 300;
constexpr int kTripleLineScore = 500;
constexpr int kTetrisScore = 800;

int ScoreForLines(int cleared) {
    switch (cleared) {
    case 1:
        return kSingleLineScore;
    case 2:
        return kDoubleLineScore;
    case 3:
        return kTripleLineScore;
    case 4:
        return kTetrisScore;
    default:
        return 0;
    }
}

} // namespace

bool GameModel::Spawn(TetrominoType type) {
    Tetromino piece{type, 0};
    if (!board_.CanPlace(piece, kSpawnX, kSpawnY)) {
        current_.reset();
        gameOver_ = true;
        return false;
    }
    current_ = ActivePiece{piece, {kSpawnX, kSpawnY}};
    gameOver_ = false;
    return true;
}

bool GameModel::Move(int dx, int dy) {
    if (!current_) {
        return false;
    }
    ActivePiece next = *current_;
    next.origin.x += dx;
    next.origin.y += dy;
    if (!CanPlaceAt(next.piece, next.origin)) {
        return false;
    }
    current_ = next;
    return true;
}

bool GameModel::RotateCW() {
    if (!current_) {
        return false;
    }
    ActivePiece next = *current_;
    next.piece.rotation = (next.piece.rotation + 1) % 4;
    if (!CanPlaceAt(next.piece, next.origin)) {
        return false;
    }
    current_ = next;
    return true;
}

bool GameModel::SoftDrop() {
    const bool moved = Move(0, 1);
    if (!moved) {
        LockPiece();
    }
    return moved;
}

void GameModel::HardDrop() {
    if (!current_) {
        return;
    }
    while (Move(0, 1)) {
    }
    LockPiece();
}

void GameModel::TickGravity() {
    if (!current_ || gameOver_) {
        return;
    }
    if (!Move(0, 1)) {
        LockPiece();
    }
}

bool GameModel::IsGameOver() const {
    return gameOver_;
}

int GameModel::Level() const {
    return level_;
}

int GameModel::LinesCleared() const {
    return linesCleared_;
}

int GameModel::Score() const {
    return score_;
}

int GameModel::GravityDelayMs() const {
    const int scaled = kBaseGravityMs - level_ * kGravityStepMs;
    return scaled < kMinGravityMs ? kMinGravityMs : scaled;
}

const Board &GameModel::GetBoard() const {
    return board_;
}

const std::optional<ActivePiece> &GameModel::CurrentPiece() const {
    return current_;
}

bool GameModel::CanPlaceAt(const Tetromino &piece, const Point &origin) const {
    return board_.CanPlace(piece, origin.x, origin.y);
}

void GameModel::LockPiece() {
    if (!current_) {
        return;
    }
    board_.Place(current_->piece, current_->origin.x, current_->origin.y);
    current_.reset();
    const int cleared = board_.ClearFullLines();
    if (cleared > 0) {
        score_ += ScoreForLines(cleared) * (level_ + 1);
        linesCleared_ += cleared;
        UpdateLevel();
    }
}

void GameModel::UpdateLevel() {
    level_ = linesCleared_ / kLinesPerLevel;
}

} // namespace mctetris::model
