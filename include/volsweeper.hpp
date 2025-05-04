#include <algorithm>
#include <random>
#include <vector>
// we won't use "using namespace std" because
// that's bad form when making header files

// struct for generating fields
struct Minefield {
    size_t size;
    size_t num_mines;
    // entry will be -1 for mines and otherwise represent number of mines adjacent
    std::vector<std::vector<int> > grid;
    std::vector<std::vector<bool> > revealed;
	std::vector<std::vector<bool> > flags;
    std::vector<std::vector<bool> > flagged;
    void increment_grid_entry(int i, int j);
    Minefield(std::string flag, size_t size_c, size_t num_mines_c);
    void debug_output_field();
	std::string debug_output_string(); // the "debug" methods return what the board actually looks like
    std::string output_string(); // the other output methods only show revealed squares
    void output_field();
    void reveal_square(int x, int y);
    void flag_square(int x, int y);
};
