//Version one of the pattern recognizing bot to show playable moves

#include "volsweeper.hpp"
#include <iostream>

std::vector<std::pair<int, int>> get_unopened_neighbors(const Minefield& field, int i, int j) {
    std::vector<std::pair<int, int>> neighbors;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            int nx = i + x, ny = j + y;
            // Check bounds and unopened cell
            if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size && field.grid[nx][ny] == 0) {
                neighbors.push_back({nx, ny});
            }
        }
    }
    return neighbors;
}


//marking mines
void mark_mines(Minefield& field, std::vector<std::pair<int, int>> neighbors) {
    for (const std:: pair<int,int>& cell : neighbors) {
        int x = cell.first, y = cell.second;
        field.grid[x][y] = -1; // Mark as mine
    }
}

//B1 (If the number of the square is the same as the adjacent squares then all of the squares are mines)
void pattern_recognition(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            int val = field.grid[i][j];
            if (val > 0) { // Check only for revealed cells with a number
				std::vector<std::pair<int,int>> unopened_neighbors = get_unopened_neighbors(field, i, j);
                if (unopened_neighbors.size() == val) {
                    mark_mines(field, unopened_neighbors);
                }
            }
        }
    }
}

//B2
//If the number is touching the same number of flags then all adjacent squares can be opened

//1-1
//Look at the left 1. It touches the two yellow cells, so they contain one mine. Now look at the right 1.
//It also touches the two yellow cells, so it already has a mine and the third cell can be opened.

//1-1+
//

//1-2
//

//1-2+
//


//1-2C
//

//1-2C+
//

//1-2-1
//

//1-2-2-1
//

int main(int argc, char* argv[]) {

	if (argc != 4) {
        std::cerr << "usage: ./volsweeper [FLAG] [SIZE] [NUM_MINES]\n";
        return 1;
    }

	int size = std::stoi(argv[2]);
    int num_mines = std::stoi(argv[3]);

    Minefield field(argv[1], size, num_mines);
	//Initial State
    field.output_field();

    // Run pattern recognition to mark obvious mines
    pattern_recognition(field);

    std::cout << "\nAfter marking mines:\n";
    field.output_field();  // Updated state

    return 0;
}
