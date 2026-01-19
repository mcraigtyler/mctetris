#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <optional>
#include <random>
#include <string>
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

struct ControlScheme {
    const char *name;
    int left;
    int right;
    int down;
    int rotate;
    int hardDrop;
    const char *hint;
};

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

void RenderGame(const mctetris::model::GameModel &model,
                const std::optional<mctetris::model::TetrominoType> &nextType,
                const ControlScheme &scheme,
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

    mvprintw(0, 0, "Score: %d  Level: %d  Lines: %d  Scheme: %s", model.Score(), model.Level(),
             model.LinesCleared(), scheme.name);
    mvprintw(kBoardOffsetY + boardHeightChars + 2, 0,
             "%s  P: pause  Q: quit", scheme.hint);

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

void RenderMenu(int selectedIndex) {
    constexpr std::array<const char *, 3> kItems = {"Start Game", "Control Scheme", "Quit"};
    const std::string title = "MCTETRIS";
    int maxWidth = static_cast<int>(title.size());
    for (const char *item : kItems) {
        maxWidth = std::max(maxWidth, static_cast<int>(std::strlen(item)));
    }
    const int startY = (LINES / 2) - static_cast<int>(kItems.size());
    const int startX = (COLS - maxWidth) / 2;

    erase();
    attron(A_BOLD);
    mvprintw(startY - 2, (COLS - static_cast<int>(title.size())) / 2, "%s", title.c_str());
    attroff(A_BOLD);
    for (size_t i = 0; i < kItems.size(); ++i) {
        if (static_cast<int>(i) == selectedIndex) {
            attron(A_REVERSE);
        }
        mvprintw(startY + static_cast<int>(i), startX, "%s", kItems[i]);
        if (static_cast<int>(i) == selectedIndex) {
            attroff(A_REVERSE);
        }
    }
    mvprintw(LINES - 2, 2, "Use Up/Down and Enter to select.");
    wnoutrefresh(stdscr);
    doupdate();
}

void RenderControlMenu(int selectedIndex,
                       int activeIndex,
                       const std::array<ControlScheme, 3> &schemes) {
    const std::string title = "CONTROL SCHEME";
    int maxWidth = static_cast<int>(title.size());
    for (const auto &scheme : schemes) {
        maxWidth = std::max(maxWidth, static_cast<int>(std::strlen(scheme.name)) + 3);
    }
    const int startY = (LINES / 2) - static_cast<int>(schemes.size());
    const int startX = (COLS - maxWidth) / 2;

    erase();
    attron(A_BOLD);
    mvprintw(startY - 2, (COLS - static_cast<int>(title.size())) / 2, "%s", title.c_str());
    attroff(A_BOLD);
    for (size_t i = 0; i < schemes.size(); ++i) {
        const bool isSelected = static_cast<int>(i) == selectedIndex;
        const bool isActive = static_cast<int>(i) == activeIndex;
        if (isSelected) {
            attron(A_REVERSE);
        }
        mvprintw(startY + static_cast<int>(i), startX, "%s%s",
                 isActive ? "* " : "  ", schemes[i].name);
        if (isSelected) {
            attroff(A_REVERSE);
        }
    }
    mvprintw(LINES - 3, 2, "Enter selects. B or Esc returns.");
    mvprintw(LINES - 2, 2, "Current scheme is marked with *.");
    wnoutrefresh(stdscr);
    doupdate();
}

mctetris::model::TetrominoType RandomType(std::mt19937 &rng) {
    std::uniform_int_distribution<int> dist(0, 6);
    return static_cast<mctetris::model::TetrominoType>(dist(rng));
}

bool IsEnterKey(int ch) {
    return ch == '\n' || ch == '\r' || ch == KEY_ENTER;
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

    const std::array<ControlScheme, 3> schemes = {
        ControlScheme{"WASD", 'a', 'd', 's', 'w', ' ', "WASD: move/rotate  Space: hard drop"},
        ControlScheme{"Arrows", KEY_LEFT, KEY_RIGHT, KEY_DOWN, KEY_UP, ' ',
                      "Arrows: move/rotate  Space: hard drop"},
        ControlScheme{"NumPad", '4', '6', '5', '8', '0',
                      "NumPad 4/6/5/8: move/rotate  0: hard drop"}};

    mctetris::model::GameModel model;
    std::random_device device;
    std::mt19937 rng(device());
    std::optional<mctetris::model::TetrominoType> nextType;

    using Clock = std::chrono::steady_clock;
    auto lastGravity = Clock::now();
    int activeScheme = 0;
    int menuIndex = 0;
    int controlIndex = 0;
    bool paused = false;
    enum class Screen { Menu,
                        Controls,
                        Game };
    Screen screen = Screen::Menu;

    auto startGame = [&]() {
        model = mctetris::model::GameModel{};
        nextType = RandomType(rng);
        (void)model.Spawn(*nextType);
        nextType = RandomType(rng);
        paused = false;
        lastGravity = Clock::now();
    };
    bool running = true;
    while (running) {
        const int ch = getch();
        if (screen == Screen::Menu) {
            if (ch == 'q' || ch == 'Q') {
                running = false;
            } else if (ch == KEY_UP || ch == 'w' || ch == 'W') {
                menuIndex = (menuIndex + 2) % 3;
            } else if (ch == KEY_DOWN || ch == 's' || ch == 'S') {
                menuIndex = (menuIndex + 1) % 3;
            } else if (IsEnterKey(ch)) {
                if (menuIndex == 0) {
                    startGame();
                    screen = Screen::Game;
                } else if (menuIndex == 1) {
                    controlIndex = activeScheme;
                    screen = Screen::Controls;
                } else if (menuIndex == 2) {
                    running = false;
                }
            }
        } else if (screen == Screen::Controls) {
            if (ch == 27 || ch == 'b' || ch == 'B') {
                screen = Screen::Menu;
            } else if (ch == KEY_UP || ch == 'w' || ch == 'W') {
                controlIndex = (controlIndex + 2) % 3;
            } else if (ch == KEY_DOWN || ch == 's' || ch == 'S') {
                controlIndex = (controlIndex + 1) % 3;
            } else if (IsEnterKey(ch)) {
                activeScheme = controlIndex;
                screen = Screen::Menu;
            }
        } else if (screen == Screen::Game) {
            if (ch == 'p' || ch == 'P') {
                paused = !paused;
            } else if (ch == 'q' || ch == 'Q') {
                running = false;
            } else if (!paused && !model.IsGameOver()) {
                const ControlScheme &scheme = schemes[activeScheme];
                if (ch == scheme.left) {
                    (void)model.Move(-1, 0);
                } else if (ch == scheme.right) {
                    (void)model.Move(1, 0);
                } else if (ch == scheme.down) {
                    (void)model.SoftDrop();
                } else if (ch == scheme.rotate) {
                    (void)model.RotateCW();
                } else if (ch == scheme.hardDrop) {
                    model.HardDrop();
                }
            }
        }

        if (screen == Screen::Game) {
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

            RenderGame(model, nextType, schemes[activeScheme], paused);
        } else if (screen == Screen::Menu) {
            RenderMenu(menuIndex);
        } else {
            RenderControlMenu(controlIndex, activeScheme, schemes);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    endwin();
    return 0;
}
