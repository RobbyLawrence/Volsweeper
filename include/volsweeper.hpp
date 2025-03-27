#include <iostream>
#include <vector>
#include <algorithm>
// we won't use "using namespace std" because
// that's bad form when making header files
struct Minefield {
    size_t size;
    std::vector<std::vector<int> > grid;
    Minefield(std::string flag) {

    }
};
