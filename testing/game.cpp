#include <vector>
#include <random>
#include <ctime>
#include <stdexcept> // Required for std::runtime_error
#include <emscripten/bind.h> // Include Embind header

// Represents the state of a single cell
enum class CellState {
    HIDDEN,
    REVEALED,
    FLAGGED
};

// Represents the content of a revealed cell or the hidden state
enum class CellValue {
    EMPTY = 0, // Use 0-8 for adjacent mine counts
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    MINE = 9,
    HIDDEN = 10, // Represents a hidden cell for JS
    FLAGGED = 11, // Represents a flagged cell for JS
    MINE_REVEALED = 12 // Represents a mine after game over
};

struct Cell {
    bool has_mine = false;
    CellState state = CellState::HIDDEN;
    int adjacent_mines = 0;
};

enum class GameStatus {
    PLAYING,
    WON,
    LOST
};

class MinesweeperGame {
private:
    int rows;
    int cols;
    int mine_count;
    std::vector<std::vector<Cell>> board;
    GameStatus status;
    int revealed_count;
    int flag_count;
    bool first_click;

    // Helper function to check if coordinates are valid
    bool isValid(int r, int c) const {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }

    // Place mines randomly, avoiding the first click location
    void placeMines(int first_r, int first_c) {
        std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
        int mines_placed = 0;
        while (mines_placed < mine_count) {
            int r = rng() % rows;
            int c = rng() % cols;

            // Don't place on the first click or where a mine already exists
            if ((r == first_r && c == first_c) || board[r][c].has_mine) {
                continue;
            }

            board[r][c].has_mine = true;
            mines_placed++;
        }
    }

    // Calculate adjacent mine counts for all non-mine cells
    void calculateAdjacentMines() {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (board[r][c].has_mine) continue;

                int count = 0;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr;
                        int nc = c + dc;
                        if (isValid(nr, nc) && board[nr][nc].has_mine) {
                            count++;
                        }
                    }
                }
                board[r][c].adjacent_mines = count;
            }
        }
    }

    // Recursive function to reveal empty adjacent cells
    void revealAdjacent(int r, int c) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nr = r + dr;
                int nc = c + dc;
                if (isValid(nr, nc)) {
                    revealCellInternal(nr, nc); // Call internal reveal
                }
            }
        }
    }

    // Internal reveal logic without first click handling
    void revealCellInternal(int r, int c) {
        if (!isValid(r, c) || board[r][c].state == CellState::REVEALED || board[r][c].state == CellState::FLAGGED) {
            return; // Already revealed, flagged, or invalid
        }

        board[r][c].state = CellState::REVEALED;
        revealed_count++;

        if (board[r][c].has_mine) {
            // This should technically only happen on the first click if handled poorly,
            // or if revealCellInternal is called directly on a mine.
            // The main revealCell handles game over logic.
            status = GameStatus::LOST; // Mark as lost, though game over is handled in revealCell
            return;
        }

        // If the revealed cell is empty (0 adjacent mines), reveal its neighbors
        if (board[r][c].adjacent_mines == 0) {
            revealAdjacent(r, c);
        }

        // Check for win condition after each reveal
        checkWinCondition();
    }

     void checkWinCondition() {
        if (status == GameStatus::PLAYING && revealed_count == (rows * cols - mine_count)) {
            status = GameStatus::WON;
             // Auto-flag remaining mines on win (optional)
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    if (board[r][c].has_mine && board[r][c].state != CellState::FLAGGED) {
                         board[r][c].state = CellState::FLAGGED;
                         flag_count++;
                    }
                }
            }
        }
    }


public:
    // Constructor exposed to JS
    MinesweeperGame(int r, int c, int mines) : rows(r), cols(c), mine_count(mines), status(GameStatus::PLAYING), revealed_count(0), flag_count(0), first_click(true) {
        if (rows <= 0 || cols <= 0 || mine_count <= 0 || mine_count >= rows * cols) {
             throw std::runtime_error("Invalid game parameters");
        }
        // Initialize the board (defer mine placement until first click)
        board.assign(rows, std::vector<Cell>(cols));
    }

    // Initialize or reset the game state (used for reset button)
    void initGame() {
        status = GameStatus::PLAYING;
        revealed_count = 0;
        flag_count = 0;
        first_click = true;
        board.assign(rows, std::vector<Cell>(cols)); // Clear board
    }

    // Method called when a user clicks to reveal a cell
    void revealCell(int r, int c) {
        if (!isValid(r, c) || status != GameStatus::PLAYING) {
            return; // Ignore clicks if game is over or invalid coords
        }

        // Handle first click: Place mines *after* knowing the first click location
        if (first_click) {
            placeMines(r, c);
            calculateAdjacentMines();
            first_click = false;
        }

        Cell& cell = board[r][c];

        // Ignore clicks on revealed or flagged cells
        if (cell.state == CellState::REVEALED || cell.state == CellState::FLAGGED) {
            return;
        }

        // --- Reveal Logic ---
        cell.state = CellState::REVEALED;

        if (cell.has_mine) {
            status = GameStatus::LOST;
            // Optionally reveal all mines on loss
            // for (auto& row_vec : board) {
            //     for (auto& cell_inner : row_vec) {
            //         if (cell_inner.has_mine) cell_inner.state = CellState::REVEALED;
            //     }
            // }
            return; // Game Over
        }

        revealed_count++;

        // If the revealed cell is empty, reveal neighbors
        if (cell.adjacent_mines == 0) {
            revealAdjacent(r, c);
        }

        // Check win condition (must be done *after* potential recursive reveals)
        checkWinCondition();
    }

    // Method called when a user right-clicks to flag/unflag a cell
    void toggleFlag(int r, int c) {
         if (!isValid(r, c) || status != GameStatus::PLAYING) {
            return; // Ignore if game is over or coords invalid
        }

        Cell& cell = board[r][c];

        if (cell.state == CellState::REVEALED) {
            return; // Cannot flag revealed cells
        }

        if (cell.state == CellState::HIDDEN) {
            cell.state = CellState::FLAGGED;
            flag_count++;
        } else if (cell.state == CellState::FLAGGED) {
            cell.state = CellState::HIDDEN;
            flag_count--;
        }
         // Note: Win condition is checked on reveal, not flagging.
    }

    // Get the current state of the board to send to JavaScript
    // Returns a flat vector representing the visible state of each cell
    std::vector<CellValue> getBoardState() const {
        std::vector<CellValue> state_vec;
        state_vec.reserve(rows * cols); // Pre-allocate memory

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                const Cell& cell = board[r][c];
                if (status == GameStatus::LOST && cell.has_mine && cell.state != CellState::FLAGGED) {
                     state_vec.push_back(CellValue::MINE_REVEALED); // Show mines on loss
                } else if (cell.state == CellState::FLAGGED) {
                    state_vec.push_back(CellValue::FLAGGED);
                } else if (cell.state == CellState::HIDDEN) {
                    state_vec.push_back(CellValue::HIDDEN);
                } else { // Revealed state
                    if (cell.has_mine) {
                        // Should only happen if a mine was clicked (loss)
                        state_vec.push_back(CellValue::MINE_REVEALED);
                    } else {
                        // Cast adjacent mine count to CellValue
                         state_vec.push_back(static_cast<CellValue>(cell.adjacent_mines));
                    }
                }
            }
        }
        return state_vec;
    }

     // Getters for basic info
     int getRows() const { return rows; }
     int getCols() const { return cols; }
     GameStatus getGameStatus() const { return status; }
     int getMineCount() const { return mine_count; }
     int getFlagCount() const { return flag_count; }
};

// --- Emscripten Bindings ---
// Expose the C++ classes and methods to JavaScript
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::enum_<GameStatus>("GameStatus")
        .value("PLAYING", GameStatus::PLAYING)
        .value("WON", GameStatus::WON)
        .value("LOST", GameStatus::LOST);

     emscripten::enum_<CellValue>("CellValue")
        .value("EMPTY", CellValue::EMPTY)
        .value("ONE", CellValue::ONE)
        .value("TWO", CellValue::TWO)
        .value("THREE", CellValue::THREE)
        .value("FOUR", CellValue::FOUR)
        .value("FIVE", CellValue::FIVE)
        .value("SIX", CellValue::SIX)
        .value("SEVEN", CellValue::SEVEN)
        .value("EIGHT", CellValue::EIGHT)
        .value("MINE", CellValue::MINE) // Technically not used directly in getBoardState display logic but good to have
        .value("HIDDEN", CellValue::HIDDEN)
        .value("FLAGGED", CellValue::FLAGGED)
        .value("MINE_REVEALED", CellValue::MINE_REVEALED);


    emscripten::class_<MinesweeperGame>("MinesweeperGame")
        .constructor<int, int, int>() // Expose constructor (rows, cols, mines)
        .function("initGame", &MinesweeperGame::initGame)
        .function("revealCell", &MinesweeperGame::revealCell)
        .function("toggleFlag", &MinesweeperGame::toggleFlag)
        .function("getBoardState", &MinesweeperGame::getBoardState)
        .function("getRows", &MinesweeperGame::getRows)
        .function("getCols", &MinesweeperGame::getCols)
        .function("getGameStatus", &MinesweeperGame::getGameStatus)
        .function("getMineCount", &MinesweeperGame::getMineCount)
        .function("getFlagCount", &MinesweeperGame::getFlagCount);

    // Need to register std::vector<CellValue> because it's used as a return type
    emscripten::register_vector<CellValue>("VectorCellValue");
}
