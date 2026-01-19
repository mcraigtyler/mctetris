#include <array>
#include <chrono>
#include <cstring>
#include <optional>
#include <random>
#include <thread>

#include <curses.h>

#include "model/game_model.h"

namespace {

constexpr int kBoardOffsetX = 2;
constexpr int kBoardOffsetY = 1;
constexpr int kCellWidth = 4;
constexpr int kCellHeight = 2;
constexpr int kPanelOffsetX = 16;
constexpr int kNextPanelWidth = kCellWidth * 4 + 4;
constexpr int kNextPanelHeight = kCellHeight * 4 + 4;
constexpr int kStatsPanelWidth = 18;
constexpr int kStatsPanelHeight = 6;

short ColorPairForCell(mctetris::model::Cell cell) {
    using mctetris::model::Cell;
    switch (cell) {
    case Cell::I:
        return 1;
    case Cell::O:
        return 2;
    case Cell::T:
        return 3;
    case Cell::S:
        return 4;
    case Cell::Z:
        return 5;
    case Cell::J:
        return 6;
    case Cell::L:
        return 7;
    case Cell::Empty:
        return 0;
    }
    return 0;
}

void DrawBox(int top, int left, int height, int width) {
    const int bottom = top + height - 1;
    const int right = left + width - 1;
    mvaddch(top, left, ACS_ULCORNER);
    mvaddch(top, right, ACS_URCORNER);
    mvaddch(bottom, left, ACS_LLCORNER);
    mvaddch(bottom, right, ACS_LRCORNER);
    for (int x = left + 1; x < right; ++x) {
        mvaddch(top, x, ACS_HLINE);
        mvaddch(bottom, x, ACS_HLINE);
    }
    for (int y = top + 1; y < bottom; ++y) {
        mvaddch(y, left, ACS_VLINE);
        mvaddch(y, right, ACS_VLINE);
    }
}

void DrawCell(int screenY, int screenX, mctetris::model::Cell cell) {
    const short pair = ColorPairForCell(cell);
    if (pair != 0) {
        attron(COLOR_PAIR(pair));
    }
    for (int dy = 0; dy < kCellHeight; ++dy) {
        for (int dx = 0; dx < kCellWidth; ++dx) {
            mvaddch(screenY + dy, screenX + dx, ' ');
        }
    }
    if (pair != 0) {
        attroff(COLOR_PAIR(pair));
    }
}

void RenderNextPiecePanel(int top, int left, const std::optional<mctetris::model::TetrominoType> &nextType) {
    DrawBox(top, left, kNextPanelHeight, kNextPanelWidth);
    mvprintw(top, left + 2, "NEXT");
    if (!nextType) {
        return;
    }

    mctetris::model::Tetromino preview{*nextType, 0};
    std::array<std::array<mctetris::model::Cell, 4>, 4> previewCells{};
    for (auto &row : previewCells) {
        row.fill(mctetris::model::Cell::Empty);
    }
    for (const auto &block : preview.Blocks()) {
        if (block.y >= 0 && block.y < 4 && block.x >= 0 && block.x < 4) {
            previewCells[block.y][block.x] = preview.CellType();
        }
    }

    const int offsetY = top + 2;
    const int offsetX = left + 2;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            DrawCell(offsetY + y * kCellHeight, offsetX + x * kCellWidth, previewCells[y][x]);
        }
    }
}

void RenderStatsPanel(int top, int left, const mctetris::model::GameModel &model) {
    DrawBox(top, left, kStatsPanelHeight, kStatsPanelWidth);
    mvprintw(top, left + 2, "STATS");
    mvprintw(top + 2, left + 2, "Score: %d", model.Score());
    mvprintw(top + 3, left + 2, "Level: %d", model.Level());
    mvprintw(top + 4, left + 2, "Lines: %d", model.LinesCleared());
}

void RenderOverlay(int centerY, int centerX, const char *text) {
    mvprintw(centerY, centerX - static_cast<int>(strlen(text)) / 2, "%s", text);
}

void Render(const mctetris::model::GameModel &model,
            const std::optional<mctetris::model::TetrominoType> &nextType,
            bool paused) {
    using mctetris::model::kBoardHeight;
    using mctetris::model::kBoardWidth;

    const int boardHeightChars = kBoardHeight * kCellHeight;
    const int boardWidthChars = kBoardWidth * kCellWidth;
    std::array<std::array<mctetris::model::Cell, kBoardWidth>, kBoardHeight> buffer{};
    const auto &cells = model.GetBoard().Cells();
    for (int y = 0; y < kBoardHeight; ++y) {
        for (int x = 0; x < kBoardWidth; ++x) {
            buffer[y][x] = cells[y][x];
        }
    }

    if (model.CurrentPiece()) {
        const auto &active = *model.CurrentPiece();
        for (const auto &block : active.piece.Blocks()) {
            const int x = active.origin.x + block.x;
            const int y = active.origin.y + block.y;
            if (x >= 0 && x < kBoardWidth && y >= 0 && y < kBoardHeight) {
                buffer[y][x] = active.piece.CellType();
            }
        }
    }

    erase();
    DrawBox(kBoardOffsetY - 1, kBoardOffsetX - 1, boardHeightChars + 2, boardWidthChars + 2);
    for (int y = 0; y < kBoardHeight; ++y) {
        for (int x = 0; x < kBoardWidth; ++x) {
            DrawCell(kBoardOffsetY + y * kCellHeight, kBoardOffsetX + x * kCellWidth, buffer[y][x]);
        }
    }

    const int panelLeft = kBoardOffsetX + boardWidthChars + kPanelOffsetX;
    const int nextTop = kBoardOffsetY;
    RenderNextPiecePanel(nextTop, panelLeft, nextType);

    const int statsTop = nextTop + kNextPanelHeight + 1;
    RenderStatsPanel(statsTop, panelLeft, model);

    mvprintw(0, 0, "Score: %d  Level: %d  Lines: %d", model.Score(), model.Level(),
             model.LinesCleared());
    mvprintw(kBoardOffsetY + boardHeightChars + 2, 0,
             "Arrows: move/rotate  Space: hard drop  P: pause  Q: quit");

    const int centerY = kBoardOffsetY + boardHeightChars / 2;
    const int centerX = kBoardOffsetX + boardWidthChars / 2;
    if (paused) {
        RenderOverlay(centerY, centerX, "PAUSED");
    } else if (model.IsGameOver()) {
        RenderOverlay(centerY, centerX, "GAME OVER");
    }
    wnoutrefresh(stdscr);
    doupdate();
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
    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(1, COLOR_CYAN, COLOR_CYAN);
        init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(4, COLOR_GREEN, COLOR_GREEN);
        init_pair(5, COLOR_RED, COLOR_RED);
        init_pair(6, COLOR_BLUE, COLOR_BLUE);
        init_pair(7, COLOR_WHITE, COLOR_WHITE);
    }

    mctetris::model::GameModel model;
    std::random_device device;
    std::mt19937 rng(device());
    std::optional<mctetris::model::TetrominoType> nextType = RandomType(rng);
    (void)model.Spawn(*nextType);
    nextType = RandomType(rng);

    using Clock = std::chrono::steady_clock;
    auto lastGravity = Clock::now();
    bool running = true;
    bool paused = false;
    while (running) {
        const int ch = getch();
        if (ch == 'p' || ch == 'P') {
            paused = !paused;
        } else if (ch == 'q' || ch == 'Q') {
            running = false;
        } else if (!paused && !model.IsGameOver()) {
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
            default:
                break;
            }
        }

        if (!paused && !model.CurrentPiece() && !model.IsGameOver()) {
            (void)model.Spawn(nextType.value_or(RandomType(rng)));
            nextType = RandomType(rng);
        }

        const auto now = Clock::now();
        const auto gravityDelay = std::chrono::milliseconds(model.GravityDelayMs());
        if (!paused && !model.IsGameOver() && now - lastGravity >= gravityDelay) {
            model.TickGravity();
            lastGravity = now;
        }

        Render(model, nextType, paused);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    endwin();
    return 0;
}
