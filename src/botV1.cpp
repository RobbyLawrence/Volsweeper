//Version one of the pattern recognizing bot to show playable moves

#include "volsweeper.hpp"
#include <iostream>

std::vector<std::pair<int, int>> get_unopened_neighbors(const Minefield& field, int i, int j) {
    std::vector<std::pair<int, int>> neighbors;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
			if (x== 0 && y == 0) continue;
            int nx = i + x, ny = j + y;
            // Check bounds and unopened cell
            if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size && !field.grid[nx][ny] == 0) {
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
		std::cout << "Flagging mine at (" << x << "," << y << ")\n";
        field.flags[x][y] = true; // Mark as mine
    }
}

//B1 (If the number of the square is the same as the adjacent squares then all of the squares are mines)
void B1(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            int val = field.grid[i][j];
            if (val > 0 && field.revealed[i][j]) { // Check only for revealed cells with a number
				std::vector<std::pair<int,int>> unopened_neighbors = get_unopened_neighbors(field, i, j);
				std::cout<< "Checking (" << i << "," << j << "), val=" << val << ",unopened_neighbors=" << unopened_neighbors.size() << ": ";
				for (const std::pair<int, int>& n : unopened_neighbors) {
					std::cout << "(" << n.first << "," << n.second << ") ";
				}
				std::cout << "\n";
				if (unopened_neighbors.size() == val) {
					std::cout << "B1 triggered at (" << i << "," << j << "), marking mines\n";
                    mark_mines(field, unopened_neighbors);
                }
            }
        }
    }
}

//B2
//If the number is touching the same number of flags then all adjacent squares can be opened
void B2(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            int val = field.grid[i][j];
            if (val > 0) {
                int flagged_neighbors = 0;
                std::vector<std::pair<int, int>> unopened_neighbors;

                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = i + dx;
                        int ny = j + dy;
                        if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size) {
                            if (field.flags[nx][ny]) flagged_neighbors++;
                            else if (!field.revealed[nx][ny]) unopened_neighbors.push_back({nx, ny});
                        }
                    }
                }

                if (flagged_neighbors == val) {
                    for (std::vector<std::pair<int, int> >::iterator it = unopened_neighbors.begin(); it != unopened_neighbors.end(); it++) {
                        field.reveal_square(it->first, it->second);
                    }
                }
            }
        }
    }
}



//1-1
//Look at the left 1. It touches the two yellow cells, so they contain one mine. Now look at the right 1.
//It also touches the two yellow cells, so it already has a mine and the third cell can be opened.
void pattern_1_1(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size - 1; j++) { // horizontal pairs
            if (field.grid[i][j] == 1 && field.grid[i][j + 1] == 1) {
                std::vector<std::pair<int, int>> shared;

                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = 0; dy <= 1; dy++) {
                        int nx1 = i + dx, ny1 = j + dy;
                        int nx2 = i + dx, ny2 = j + 1 + dy;
                        if (nx1 >= 0 && nx1 < field.size && ny1 >= 0 && ny1 < field.size &&
                            nx2 >= 0 && nx2 < field.size && ny2 >= 0 && ny2 < field.size) {
                            if (!field.revealed[nx1][ny1] && !field.revealed[nx2][ny2] && nx1 == nx2) {
                                shared.push_back({nx1, ny1});
                                shared.push_back({nx2, ny2});
                            }
                        }
                    }
                }

                if (shared.size() == 3) {
                    for (std::vector<std::pair<int, int> >:: iterator it = shared.begin(); it != shared.end(); it++) {
                        if (!field.flags[it->first][it->second]) {
                            field.reveal_square(it->first, it->second);
                        }
                    }
                }
            }
        }
    }
}




//1-1+
//

//1-2
//
void pattern_1_2(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size - 1; j++) {
            if ((field.grid[i][j] == 1 && field.grid[i][j + 1] == 2) ||
                (field.grid[i][j] == 2 && field.grid[i][j + 1] == 1)) {
                std::vector<std::pair<int, int>> adjacent_unopened;

                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = 0; dy <= 1; dy++) {
                        int nx = i + dx;
                        int ny = j + dy;
                        if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size &&
                            !field.revealed[nx][ny]) {
                            adjacent_unopened.push_back({nx, ny});
                        }
                    }
                }

                if (adjacent_unopened.size() >= 2) {
                    field.reveal_square(adjacent_unopened[0].first, adjacent_unopened[0].second);
                }
            }
        }
    }
}

//1-2+
//


//1-2C
//
void pattern_1_2_corner(Minefield& field) {
    for (int i = 0; i < field.size - 1; i++) {
        for (int j = 0; j < field.size - 1; j++) {
            if ((field.grid[i][j] == 1 && field.grid[i+1][j+1] == 2) ||
                (field.grid[i+1][j+1] == 1 && field.grid[i][j] == 2)) {
                // check cells adjacent to both
                // reveal safe tiles here
            }
        }
    }
}

//1-2C+
//

//1-2-1
//
void pattern_1_2_1(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size - 2; j++) {
            if (field.grid[i][j] == 1 && field.grid[i][j+1] == 2 && field.grid[i][j+2] == 1) {
                // apply 1-2-1 rule
                // mark mines adjacent to center
            }
        }
    }
}

//1-2-2-1
//
void pattern_1_2_2_1(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size - 3; j++) {
            if (field.grid[i][j] == 1 && field.grid[i][j+1] == 2 &&
                field.grid[i][j+2] == 2 && field.grid[i][j+3] == 1) {
                // use extended pattern logic
            }
        }
    }
}

/*void pattern_recognition(Minefield& field) {
    B1(field);
    B2(field);
    pattern_1_1(field);
    pattern_1_2(field);
    pattern_1_2_corner(field);
    pattern_1_2_1(field);
    pattern_1_2_2_1(field);
}*/

//bot output
void bot_output_field(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            if (field.flags[i][j]) {
                std::cout << "F "; //flagged by bot
            }
            else if (field.revealed[i][j]) {
                if (field.grid[i][j] == -1) {
                    std::cout << "M "; //mine
                }
                else {
                    std::cout << field.grid[i][j] << " "; //number
                }
            }
            else {
                std::cout << "X "; //Unopened
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

// Helper function to simulate revealing and running the bot
void test_bot() {
    // 5x5 grid example (mines are marked as -1)
    Minefield field("dummy", 5, 2); 
    field.grid = {
        {1, -1, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, -1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    // Manually reveal a few squares (we're simulating the game state)
    field.revealed[0][0] = true;
	field.revealed[0][1] = false;
    field.revealed[1][0] = true;
	field.revealed[1][1] = true;

	//debug
	std::cout << "Initial grid:\n";
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            std::cout << field.grid[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "Initial revealed state:\n";
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            std::cout << (field.revealed[i][j] ? "T " : "F ");
        }
        std::cout << "\n";
    }
    std::cout << "Initial flags state:\n";
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            std::cout << (field.flags[i][j] ? "T " : "F ");
        }
        std::cout << "\n";
    }

    // Output the field before running the bot
    std::cout << "Board before bot:\n";
    field.output_field();

    // Run pattern recognition to show bot's moves (B1 logic)
    B1(field); // This will mark the mines

    // Output the field after the bot has run
    std::cout << "\nBoard after bot:\n";
    bot_output_field(field);  // Updated state
}

int main() {
    test_bot(); // Call the test function
    return 0;
}

/*int main(int argc, char* argv[]) {

	if (argc != 4) {
        std::cerr << "usage: ./volsweeper [FLAG] [SIZE] [NUM_MINES]\n";
        return 1;
    }

	int size = std::stoi(argv[2]);
    int num_mines = std::stoi(argv[3]);

    Minefield field(argv[1], size, num_mines);
	//Initial State
    field.output_field();

    // B1 to mark obvious mines
    B1(field);

	//bot output
	std::cout << "\nBot's view of the field:\n";
	bot_output_field(field);

    return 0;
}*/
