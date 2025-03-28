#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>
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
        num_mines = num_mines_c;
        size = size_c;
        std::string flag_string;
        if (flag == "-r") {
            std::random_device rd; // these two lines are for <random>
            std::mt19937 gen(rd());
            // tracker tracks whether or not a tile is available to be made a mine
            std::unordered_map<int,int> tracker; // tracker to keep track of cells that can be filled
            for (size_t i = 0;i<size*size-1;i++) {
                tracker.insert(std::make_pair(i,0));
            }
            // each tile is represented in the map, and 0 means a mine can be placed, 1 means no
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
            std::uniform_int_distribution<int> dist(0,size*size-1);
            grid.resize(size,std::vector<int>(size,0));
            int random_number;
            for (size_t i = 0;i<num_mines;i++) {
                random_number = dist(gen);
                if (grid[(int)random_number/size][random_number % size] == -1) {
                    i--; // if the randomly selected number is a mine,
                    continue;
                }
            }
        }
    }
};
