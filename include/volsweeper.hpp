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
    void increment_grid_entry(int i, int j);
    Minefield(std::string flag, size_t size_c, size_t num_mines_c);
    void output_field();
};
