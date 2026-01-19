#include <array>
#include <chrono>
#include <curses.h>
#include <random>
#include <thread>

#include "model/game_model.h"

namespace {

constexpr int kBoardOffsetX = 2;
constexpr int kBoardOffsetY = 1;

char CellToChar(mctetris::model::Cell cell) {
    return cell == mctetris::model::Cell::Empty ? ' ' : '#';
}

void Render(const mctetris::model::GameModel &model) {
    using mctetris::model::kBoardHeight;
    using mctetris::model::kBoardWidth;

    std::array<std::array<char, kBoardWidth>, kBoardHeight> buffer{};
    const auto &cells = model.GetBoard().Cells();
    for (int y = 0; y < kBoardHeight; ++y) {
        for (int x = 0; x < kBoardWidth; ++x) {
            buffer[y][x] = CellToChar(cells[y][x]);
        }
    }

    if (model.CurrentPiece()) {
        const auto &active = *model.CurrentPiece();
        for (const auto &block : active.piece.Blocks()) {
            const int x = active.origin.x + block.x;
            const int y = active.origin.y + block.y;
            if (x >= 0 && x < kBoardWidth && y >= 0 && y < kBoardHeight) {
                buffer[y][x] = '@';
            }
        }
    }

    clear();
    for (int y = 0; y < kBoardHeight; ++y) {
        mvaddch(kBoardOffsetY + y, kBoardOffsetX - 1, '|');
        for (int x = 0; x < kBoardWidth; ++x) {
            mvaddch(kBoardOffsetY + y, kBoardOffsetX + x, buffer[y][x]);
        }
        mvaddch(kBoardOffsetY + y, kBoardOffsetX + kBoardWidth, '|');
    }
    for (int x = 0; x < kBoardWidth + 2; ++x) {
        mvaddch(kBoardOffsetY + kBoardHeight, kBoardOffsetX - 1 + x, '-');
    }

    mvprintw(0, 0, "Level: %d  Lines: %d  Gravity: %dms", model.Level(), model.LinesCleared(),
             model.GravityDelayMs());
    mvprintw(kBoardOffsetY + kBoardHeight + 1, 0, "Arrows: move/rotate  Space: hard drop  Q: quit");

    if (model.IsGameOver()) {
        mvprintw(kBoardOffsetY + kBoardHeight / 2, kBoardOffsetX + 1, "GAME OVER");
    }
    refresh();
}

mctetris::model::TetrominoType RandomType(std::mt19937 &rng) {
    std::uniform_int_distribution<int> dist(0, 6);
    return static_cast<mctetris::model::TetrominoType>(dist(rng));
}

} // namespace

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    nodelay(stdscr, true);

    mctetris::model::GameModel model;
    std::random_device device;
    std::mt19937 rng(device());
    (void)model.Spawn(RandomType(rng));

    using Clock = std::chrono::steady_clock;
    auto lastGravity = Clock::now();
    bool running = true;
    while (running) {
        const int ch = getch();
        switch (ch) {
        case KEY_LEFT:
            (void)model.Move(-1, 0);
            break;
        case KEY_RIGHT:
            (void)model.Move(1, 0);
            break;
        case KEY_DOWN:
            (void)model.SoftDrop();
            break;
        case KEY_UP:
            (void)model.RotateCW();
            break;
        case ' ':
            model.HardDrop();
            break;
        case 'q':
        case 'Q':
            running = false;
            break;
        default:
            break;
        }

        if (!model.CurrentPiece() && !model.IsGameOver()) {
            (void)model.Spawn(RandomType(rng));
        }

        const auto now = Clock::now();
        const auto gravityDelay = std::chrono::milliseconds(model.GravityDelayMs());
        if (!model.IsGameOver() && now - lastGravity >= gravityDelay) {
            model.TickGravity();
            lastGravity = now;
        }

        Render(model);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    endwin();
    return 0;
}
