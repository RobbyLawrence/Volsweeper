#include "volsweeper.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// decided not to use this hash function, but am keeping it just in case I need it
struct Pair_Hash {
    std::size_t operator()(const std::pair<std::size_t,std::size_t>& p) const noexcept {
        return p.first ^ (p.second * 0x9e3779b97f4a7c15ULL); // just a combinator
    }
};

using Point = std::pair<std::size_t,std::size_t>;

void Minefield::increment_grid_entry(int i, int j) { // function to increment a square if it's not a mine
    if (grid[i][j] != -1) {
        grid[i][j]++;
    }
}

Minefield::Minefield(std::string flag, size_t size_c, size_t num_mines_c, std::pair<size_t,size_t> f_square) {
    // we'll assume num_mines is positive, so error-checking is required in the
    // main file
    // // each tile is represented in the map, and 0 means a mine can be placed, 1
    // means no a mine can go anywhere, as long as it isn't surrounded fully by
    // mines so we run a check to see if a tile is surrounded by mines
    /* Board can be visualized like this:
    _____________
    | 0 | 1 | 2 |
    -------------
    | 3 | 4 | 5 |
    -------------
    | 6 | 7 | 8 |
    -------------
    We assume all boards are squares.
    The number in each square represents the "ID" of the tile. To get the grid
    vector entry for an ID x, use grid[floor(x/size)][x % size] (i came up with
    these equations, hopefully they work). We can use type casting to integer for
    the floor function.
    Later on, I call the ID of a tile its "wrapping index".
    */
    num_mines = num_mines_c;
    size = size_c;
    std::string flag_string;
    revealed.resize(size,std::vector<bool>(size,false));
    flagged.resize(size,std::vector<bool>(size,false));
    mines.resize(size*size,false);
    if (flag == "-tr") {
        grid.resize(size, std::vector<int>(size, 0));
        std::vector<int> mine_ids(num_mines);
        std::vector<int> available_numbers(size * size);
        std::iota(available_numbers.begin(), available_numbers.end(), 0);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(available_numbers.begin(), available_numbers.end(), gen);
        std::copy_n(available_numbers.begin(), num_mines, mine_ids.begin());
        for (int mine_id : mine_ids) {
            mines[mine_id] = true;
        }
        /*
        The above code generates a vector that has num_mines unique random integers
        using <random>. Below, we use these IDs to fill the grid with mines.
        */
        for (size_t i = 0; i < mine_ids.size(); i++) {
            grid[(int)(mine_ids[i] / size)][mine_ids[i] % size] = -1;
        }
        // the grid is now filled with mines, and we want to update the squares
        // close to it. we'll only update the squares that are adjacent to mines
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == 0 && j == 0 && grid[i][j] == -1) { // top left corner is mine
                    increment_grid_entry(i + 1, j);
                    increment_grid_entry(i + 1, j + 1);
                    increment_grid_entry(i, j + 1);
                    continue;
                }
                if (i == 0 && j == size - 1 &&
                    grid[i][j] == -1) { // top right corner is mine
                        increment_grid_entry(i + 1, j);
                        increment_grid_entry(i, j - 1);
                        increment_grid_entry(i + 1, j - 1);
                        continue;
                    }
                    if (i == size - 1 && j == 0 &&
                        grid[i][j] == -1) { // bottom left corner is mine
                            increment_grid_entry(i, j + 1);
                            increment_grid_entry(i - 1, j);
                            increment_grid_entry(i - 1, j + 1);
                            continue;
                        }
                        if (i == size - 1 && j == size - 1 &&
                            grid[i][j] == -1) { // bottom right corner is mine
                                increment_grid_entry(i, j - 1);
                                increment_grid_entry(i - 1, j);
                                increment_grid_entry(i - 1, j - 1);
                                continue;
                            }
                            if (i == 0 && grid[i][j] == -1) { // top wall tile is a mine
                                increment_grid_entry(i, j - 1);
                                increment_grid_entry(i + 1, j - 1);
                                increment_grid_entry(i + 1, j);
                                increment_grid_entry(i + 1, j + 1);
                                increment_grid_entry(i, j + 1);
                                continue;
                            }
                            if (j == 0 && grid[i][j] == -1) { // left wall tile is a mine
                                increment_grid_entry(i - 1, j);
                                increment_grid_entry(i - 1, j + 1);
                                increment_grid_entry(i, j + 1);
                                increment_grid_entry(i + 1, j + 1);
                                increment_grid_entry(i + 1, j);
                                continue;
                            }
                            if (j == size - 1 && grid[i][j] == -1) { // right wall tile is a mine
                                increment_grid_entry(i - 1, j);
                                increment_grid_entry(i - 1, j - 1);
                                increment_grid_entry(i, j - 1);
                                increment_grid_entry(i + 1, j - 1);
                                increment_grid_entry(i + 1, j);
                                continue;
                            }
                            if (i == size - 1 && grid[i][j] == -1) { // bottom wall tile is a mine
                                increment_grid_entry(i, j - 1);
                                increment_grid_entry(i - 1, j - 1);
                                increment_grid_entry(i - 1, j);
                                increment_grid_entry(i - 1, j + 1);
                                increment_grid_entry(i, j + 1);
                                continue;
                            } else if (grid[i][j] == -1) { // tile is surrounded by 8 tiles
                                increment_grid_entry(i - 1, j - 1);
                                increment_grid_entry(i - 1, j);
                                increment_grid_entry(i - 1, j + 1);
                                increment_grid_entry(i, j - 1);
                                increment_grid_entry(i, j + 1);
                                increment_grid_entry(i + 1, j - 1);
                                increment_grid_entry(i + 1, j);
                                increment_grid_entry(i + 1, j + 1);
                            }
            }
        }
    }
    else if (flag == "-r") {
        // convert to 0-based
        int x0 = int(f_square.first)  - 1;
        int y0 = int(f_square.second) - 1;

        // mark the 3×3 block around the first click as “no-mine”
        std::vector<bool> is_safe(size*size, false);
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int cx = x0 + dx, cy = y0 + dy;
                if (cx >= 0 && cx < size && cy >= 0 && cy < size) {
                    int row = size - 1 - cy;    // row 0 = top
                    int col = cx;               // col 0 = left
                    is_safe[row*size + col] = true;
                }
            }
        }

        // collect all other indices
        std::vector<int> avail;
        avail.reserve(size*size);
        for (int idx = 0; idx < size*size; ++idx) {
            if (!is_safe[idx])
            avail.push_back(idx);
        }

        if (avail.size() < num_mines) {
            exit(1);
        }

        // shuffle & pick mines
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(avail.begin(), avail.end(), gen);
        std::vector<int> mine_ids(avail.begin(), avail.begin() + num_mines);

        // place mines
        grid.assign(size, std::vector<int>(size, 0));
        for (int id : mine_ids) {
            int r = id / size, c = id % size;
            grid[r][c] = -1;
        }

        // fill in neighbor counts
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (grid[i][j] != -1) continue;
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        if (di==0 && dj==0) continue;
                        int ni = i + di, nj = j + dj;
                        if (ni>=0 && ni<size && nj>=0 && nj<size)
                        increment_grid_entry(ni, nj);
                    }
                }
            }
        }
    }
}

void Minefield::debug_output_field() { // output the grid of a minefield object
    for (std::vector<int> vect : grid) {
        for (int entry : vect) {
            if (entry == -1) {
                std::cout << 'M' << "  ";
                continue;
            }
            std::cout << entry << "  ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

std::string Minefield::debug_output_string() {
    std::stringstream ss;
    for (std::vector<int> vect : grid) {
        for (int entry : vect) {
            if (entry == -1) {
                ss << 'M' << "  ";
                continue;
            }
            ss << entry << "  ";
        }
        ss << '\n';
    }
    ss << '\n';
    return ss.str();
}

void Minefield::output_field() {
    for (int i = 0; i < size; i++) {
        for (int j = 0;j<size; j++) {
            if (revealed[i][j]) {
                if (grid[i][j] == -1) {
                    std::cout << "M  ";
                }
                else {
                    std::cout << grid[i][j] << "  ";
                }
            }
            else if (!(revealed[i][j]) && flagged[i][j]) {
                std::cout << "F  ";
            }
            else {
                std::cout << "X  ";
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

std::string Minefield::output_string() {
    std::stringstream ss;
    for (int i = 0; i < size; i++) {
        for (int j = 0;j<size; j++) {
            if (revealed[i][j]) {
                if (grid[i][j] == -1) {
                    ss << "M  ";
                }
                else {
                    ss << grid[i][j] << "  ";
                }
            }
            else {
                ss << "X  ";
            }
        }
        ss << '\n';
    }
    ss << '\n';
    return ss.str();
}

void Minefield::reveal_square(int x, int y) {
    if (revealed[x][y]) { // prevent infinite recursion
        return;
    }
    revealed[x][y] = true; // mark square

    if (grid[x][y] == 0) { // recursively opening zero squares
        std::vector<int> directions = {-1, 0, 1};
        for (int direction_1 : directions) {
            for (int direction_2 : directions) {
                if (direction_1 == 0 && direction_2 == 0) {
                    continue;
                }
                int new_x = x + direction_1;
                int new_y = y + direction_2;
                if (new_x >= 0 && new_x < size && new_y >= 0 && new_y < size) {
                    reveal_square(new_x, new_y);
                }
            }
        }
    }
    return;
}

void Minefield::flag_square(int x, int y) {
    if (revealed[x][y]) {
        std::cout << "Can't flag a square that's already revealed\n";
        return; // we don't want to flag a revealed square
    }
    if (flagged[x][y]) {
        flagged[x][y] = false; // let the user unflag a square
        return;
    }
    flagged[x][y] = true;
    return;
}

bool check_status(Minefield field) {
    size_t flags_placed  = 0;
    size_t correct_flags = 0;
    size_t N = field.size;

    // 1) Count flags and correct flags
    for (size_t r = 0; r < N; ++r) {
        for (size_t c = 0; c < N; ++c) {
            if (!field.flagged[r][c])
                continue;

            ++flags_placed;

            // flatten (r,c) to index in field.mines:
            size_t idx = r * N + c;
            if (field.mines[idx])
                ++correct_flags;
        }
    }

    // if not all flags used, game still in progress
    if (flags_placed < field.num_mines) {
        return false;
    }

    // if we've used all flags, we have to check correctness
    if (correct_flags == field.num_mines) {
        std::cout << "You win the game!\n";
    } else {
        std::cout << "You've placed at least one incorrect flag!\n";
    }
    return true;  // game over
}
