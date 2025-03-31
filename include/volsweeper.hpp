#include <vector>
#include <algorithm>
#include <random>
// we won't use "using namespace std" because
// that's bad form when making header files


// struct for generating fields
struct Minefield {
    size_t size;
    size_t num_mines;
    // entry will be -1 for mines and otherwise represent number of mines adjacent
    std::vector<std::vector<int> > grid;
    Minefield(std::string flag, size_t size_c, size_t num_mines_c) {
        // we'll assume num_mines is positive, so error-checking is required in the main file
        // // each tile is represented in the map, and 0 means a mine can be placed, 1 means no
        // a mine can go anywhere, as long as it isn't surrounded fully by mines
        // so we run a check to see if a tile is surrounded by mines
        /* Board can be visualized like this:
        _____________
        | 0 | 1 | 2 |
        -------------
        | 3 | 4 | 5 |
        -------------
        | 6 | 7 | 8 |
        -------------
        We assume all boards are squares.
        The number in each square represents the "ID" of the tile. To get the grid vector entry for an ID x,
        use grid[floor(x/size)][x % size] (i came up with these equations, hopefully they work). We can use
        type casting to integer for the floor function.
        */
        num_mines = num_mines_c;
        size = size_c;
        std::string flag_string;
        if (flag == "-r") {
            grid.resize(size,std::vector<int>(size,0));
            std::vector<int> mine_ids(num_mines);
            std::vector<int> available_numbers(size*size);
            std::iota(available_numbers.begin(), available_numbers.end(), 0);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(available_numbers.begin(), available_numbers.end(), gen);
            std::copy_n(available_numbers.begin(), num_mines, mine_ids.begin());
            /*
            The above code generates a vector that has num_mines unique random integers using <random>.
            Below, we use these IDs to fill the grid with mines.
            */
            for (size_t i = 0;i<mine_ids.size();i++) {
                grid[(int)(i / size)][i % size] = -1;
            }
            // the grid is now filled with mines, and we want to update the squares close to it.
            // we'll only update the squares that are adjacent to mines
            for (int i = 0;i<size;i++) {
                for (int j = 0;j<size;j++) {
                    if (i == 0 && j == 0 && grid[i][j] == -1) { // top left corner is mine

                        continue;
                    }
                    if (i == 0 && j == size - 1 && grid[i][j]) { // top right corner is mine
                        // process
                        continue;
                    }
                    if (i == size - 1 && j == 0) { // bottom left corner is mine
                        // process
                        continue;
                    }
                    if (i == size - 1 && j == size - 1) { // bottom right corner is mine
                        // process
                        continue;
                    }
                    if (i == 0) { // top wall tile is a mine
                        // process
                        continue;
                    }
                    if (j == 0) { // left wall tile is a mine
                        // process
                        continue;
                    }
                    if (j == size - 1) { // right wall tile is a mine
                        // process
                        continue;
                    }
                    if (i == size - 1) { // bottom wall tile is a mine
                        // process
                        continue;
                    }
                }
            }

        }
    }
};
