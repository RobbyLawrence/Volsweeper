//Pattern recognizing bot to show playable moves

#include "volsweeper.hpp"
#include <iostream>

//gets the unopened neighbors of each cell
std::vector<std::pair<int, int>> get_unopened_neighbors(const Minefield& field, int i, int j) {
    std::vector<std::pair<int, int>> neighbors;
    //check all adjacent cells in a 3x3 grid
	for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
			//skip the center cell
			if (x== 0 && y == 0) continue;
            int nx = i + x, ny = j + y;
            //check bounds and unopened cell
            if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size && !field.revealed[nx][ny]) {
                neighbors.push_back({nx, ny});
            }
        }
    }
    return neighbors;
}


//function to mark mines given neighboring cells
void mark_mines(Minefield& field, std::vector<std::pair<int, int>> neighbors) {
    for (const std:: pair<int,int>& cell : neighbors) {
        int x = cell.first, y = cell.second;
		std::cout << "Flagging mine at (" << x << "," << y << ")\n";
        field.flags[x][y] = true; // Mark as mine
    }
}

//B1 (If the number of the square is the same as the adjacent squares then all of the squares are mines)
void B1(Minefield& field) {

	//loop through the board
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            int val = field.grid[i][j];
			//check only for revealed cells with a number > 0
            if (val > 0 && field.revealed[i][j]) {
				//get unopened neighbors
				std::vector<std::pair<int,int>> unopened_neighbors = get_unopened_neighbors(field, i, j);
				//std::cout<< "Checking (" << i << "," << j << "), val=" << val << ",unopened_neighbors=" << unopened_neighbors.size() << ": ";
				for (const std::pair<int, int>& n : unopened_neighbors) {
					//std::cout << "(" << n.first << "," << n.second << ") ";
				}
				//std::cout << "\n";
				//if unopened neighbors equals the value of the tile mark the unopened neighbor as a mine
				if (unopened_neighbors.size() == val) {
					std::cout << "B1 triggered at (" << i << "," << j << "), marking mines\n";
                    mark_mines(field, unopened_neighbors);
                }
            }
        }
    }
}


void B2(Minefield& field) {
    //std::cout << "Running B2 detection...\n";

    // Vector to store candidates
    std::vector<std::pair<int, int> > candidates;

    // First pass: Find candidates
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            int val = field.grid[i][j];
            // Only process revealed tiles with value > 0
            if (val > 0 && field.revealed[i][j]) {
                int flagged_neighbors = 0;
                // Count flagged neighbors
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = i + dx;
                        int ny = j + dy;
                        if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size && field.flags[nx][ny]) {
                            flagged_neighbors++;
                        }
                    }
                }
                // Debug: Log checked cells
                //std::cout << "Checking (" << i << "," << j << "), val=" << val << ", flagged_neighbors=" << flagged_neighbors << "\n";
                // If flagged neighbors equal the value, add to candidates
                if (flagged_neighbors == val) {
                    candidates.push_back(std::pair<int, int>(i, j));
                    //std::cout << "B2 candidate at (" << i << "," << j << "), flagged_neighbors=" << flagged_neighbors << "\n";
                }
            }
        }
    }

    // Second pass: Reveal safe unopened neighbors for each candidate
    for (std::vector<std::pair<int, int> >::iterator it = candidates.begin(); it != candidates.end(); ++it) {
        int i = it->first;
        int j = it->second;
        std::vector<std::pair<int, int> > unopened_neighbors = get_unopened_neighbors(field, i, j);
        std::cout << "B2 triggered at (" << i << "," << j << "), revealing safe tiles\n";
        for (std::vector<std::pair<int, int> >::iterator neighbor_it = unopened_neighbors.begin(); neighbor_it != unopened_neighbors.end(); ++neighbor_it) {
            int x = neighbor_it->first;
            int y = neighbor_it->second;
            if (!field.flags[x][y]) { // Only reveal if not flagged
                std::cout << "  Safe to reveal: (" << x + 1 << "," << y + 1 << ")\n";
                field.reveal_square(x, y);
            }
        }
    }
}

// Corrected 1-1 pattern detection
void pattern_1_1(Minefield& field) {
    std::cout << "Running pattern_1_1 detection..." << std::endl;
    bool made_move = false;

    // Check horizontal patterns (same row, adjacent columns)
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size - 1; j++) {
            if (field.revealed[i][j] && field.revealed[i][j + 1] &&
                field.grid[i][j] == 1 && field.grid[i][j + 1] == 1) {

                std::cout << "  Found horizontal 1-1 pattern at (" << i << "," << j << ") and (" << i << "," << (j+1) << ")\n";

                std::vector<std::pair<int, int>> cells_around_first;
                std::vector<std::pair<int, int>> cells_around_second;
                std::vector<std::pair<int, int>> shared_cells;
                std::vector<std::pair<int, int>> exclusive_to_first;
                std::vector<std::pair<int, int>> exclusive_to_second;

                // Cells around the first 1
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = i + dx;
                        int ny = j + 1 + dy;
                        if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size &&
                            !field.revealed[nx][ny]) {
                            cells_around_first.push_back(std::make_pair(nx, ny));
                        }
                    }
                }

                // Cells around the second 1
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = i + dx;
                        int ny = j + 1 + dy;
                        if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size &&
                            !field.revealed[nx][ny]) {
                            cells_around_second.push_back(std::make_pair(nx, ny));
                        }
                    }
                }

                std::cout << "  First 1 has " << cells_around_first.size() << " unrevealed neighbors\n";
                std::cout << "  Second 1 has " << cells_around_second.size() << " unrevealed neighbors\n";

                // Find shared and exclusive cells
                for (const auto& cell1 : cells_around_first) {
                    bool shared = false;
                    for (const auto& cell2 : cells_around_second) {
                        if (cell1.first == cell2.first && cell1.second == cell2.second) {
                            shared = true;
                            shared_cells.push_back(cell1);
                            break;
                        }
                    }
                    if (!shared) {
                        exclusive_to_first.push_back(cell1);
                    }
                }

                for (const auto& cell2 : cells_around_second) {
                    bool shared = false;
                    for (const auto& cell1 : cells_around_first) {
                        if (cell2.first == cell1.first && cell2.second == cell1.second) {
                            shared = true;
                            break;
                        }
                    }
                    if (!shared) {
                        exclusive_to_second.push_back(cell2);
                    }
                }

                std::cout << "  Shared cells: " << shared_cells.size() << "\n";
                std::cout << "  Exclusive to first: " << exclusive_to_first.size() << "\n";
                std::cout << "  Exclusive to second: " << exclusive_to_second.size() << "\n";

                // Count existing flags
                int flags_first = 0;
                int flags_second = 0;

                for (const auto& cell : cells_around_first) {
                    if (field.flags[cell.first][cell.second]) {
                        flags_first++;
                    }
                }

                for (const auto& cell : cells_around_second) {
                    if (field.flags[cell.first][cell.second]) {
                        flags_second++;
                    }
                }

                std::cout << "  First 1 has " << flags_first << " flags\n";
                std::cout << "  Second 1 has " << flags_second << " flags\n";

                // Corrected logic: Only handle exactly 2 shared cells and no flags
                if (shared_cells.size() == 2 && flags_first == 0 && flags_second == 0) {
                    std::cout << "  1-1 Pattern: Found " << shared_cells.size() << " shared cells\n";
                    std::cout << "  1-1 Pattern: Revealing exclusive cells\n";

                    // Reveal safe exclusive cells
                    for (const auto& cell : exclusive_to_first) {
                        int x = cell.first;
                        int y = cell.second;
                        if (!field.flags[x][y]) {
                            std::cout << "  Safe to reveal: (" << x << "," << y << ")\n";
                            field.reveal_square(x, y);
                            made_move = true;
                        }
                    }

                    for (const auto& cell : exclusive_to_second) {
                        int x = cell.first;
                        int y = cell.second;
                        if (!field.flags[x][y]) {
                            std::cout << "  Safe to reveal: (" << x << "," << y << ")\n";
                            field.reveal_square(x, y);
                            made_move = true;
                        }
                    }
                }
            }
        }
    }

    // Check vertical patterns (same column, adjacent rows) - same corrections applied
    for (int j = 0; j < field.size; j++) {
        for (int i = 0; i < field.size - 1; i++) {
            if (field.revealed[i][j] && field.revealed[i + 1][j] &&
                field.grid[i][j] == 1 && field.grid[i + 1][j] == 1) {

                std::cout << "  Found vertical 1-1 pattern at (" << i << "," << j << ") and (" << (i+1) << "," << j << ")\n";

                
				
				
            std::vector<std::pair<int, int>> cells_around_first;
            std::vector<std::pair<int, int>> cells_around_second;
            std::vector<std::pair<int, int>> shared_cells;
            std::vector<std::pair<int, int>> exclusive_to_first;
            std::vector<std::pair<int, int>> exclusive_to_second;

            // Cells around the first 1
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = i + dx;
                    int ny = j + dy;
                    if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size &&
                        !field.revealed[nx][ny]) {
                        cells_around_first.push_back(std::make_pair(nx, ny));
                    }
                }
            }

            // Cells around the second 1
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = i + 1 + dx;
                    int ny = j + dy;
                    if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size &&
                        !field.revealed[nx][ny]) {
                        cells_around_second.push_back(std::make_pair(nx, ny));
                    }
                }
            }

            // Count existing flags
            int flags_first = 0;
            int flags_second = 0;

            for (const auto& cell : cells_around_first) {
                if (field.flags[cell.first][cell.second]) {
                    flags_first++;
                }
            }

			for (const auto& cell : cells_around_second) {
                if (field.flags[cell.first][cell.second]) {
                    flags_second++;
                }
            }

                // Corrected logic: Only handle exactly 2 shared cells and no flags
                if (shared_cells.size() == 2 && flags_first == 0 && flags_second == 0) {
                    std::cout << "  1-1 Pattern: Found " << shared_cells.size() << " shared cells\n";
                    std::cout << "  1-1 Pattern: Revealing exclusive cells\n";

                    // Reveal safe exclusive cells
                    for (const auto& cell : exclusive_to_first) {
                        int x = cell.first;
                        int y = cell.second;
                        if (!field.flags[x][y]) {
                            std::cout << "  Safe to reveal: (" << x << "," << y << ")\n";
                            field.reveal_square(x, y);
                            made_move = true;
                        }
                    }

                    for (const auto& cell : exclusive_to_second) {
                        int x = cell.first;
                        int y = cell.second;
                        if (!field.flags[x][y]) {
                            std::cout << "  Safe to reveal: (" << x << "," << y << ")\n";
                            field.reveal_square(x, y);
                            made_move = true;
                        }
                    }
                }
            }
        }
    }

    if (!made_move) {
        std::cout << "  No valid 1-1 patterns found or no moves made\n";
    }
}

//1-2
//when two horizontal cells are labeled 1 and 2 and they share 2 unopened cells one of those
//cells must be a mine and the other can potentially be opened
void pattern_1_2(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size - 1; j++) {
			//check for 1-2 or 2-1 horizontal patterns
            if ((field.grid[i][j] == 1 && field.grid[i][j + 1] == 2) ||
                (field.grid[i][j] == 2 && field.grid[i][j + 1] == 1)) {
                std::vector<std::pair<int, int>> adjacent_unopened;

				//get unrevealed cells adjacent to the pattern
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = 0; dy <= 1; dy++) {
                        int nx = i + dx;
                        int ny = j + dy;
						//check bounds and unrevealed state
                        if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size &&
                            !field.revealed[nx][ny]) {
                            adjacent_unopened.push_back({nx, ny});
                        }
                    }
                }
				//if two or more cells are found reveal one of them
                if (adjacent_unopened.size() >= 2) {
                    field.reveal_square(adjacent_unopened[0].first, adjacent_unopened[0].second);
                }
            }
        }
    }
}

//1-2C
//when there is a 1-2 horizontal pattern there cannot be 2 mines adjacent to both of them
//so there can only be a max of one mine in the 2 squares and that guarantees that the cell only
//touching the 2 and not the 1 is a mine and should be flagged
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

//1-2-1
// 1-2-1 Pattern: Detects horizontal 1-2-1 pattern and deduces mines/safe cells
void pattern_1_2_1(Minefield& field) {
    std::cout << "Running pattern_1_2_1 detection..." << std::endl;

    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size - 2; j++) {
            // Check for revealed 1-2-1 pattern
            if (field.revealed[i][j] && field.revealed[i][j + 1] && field.revealed[i][j + 2] &&
                field.grid[i][j] == 1 && field.grid[i][j + 1] == 2 && field.grid[i][j + 2] == 1) {
                
                // Get unopened neighbors for each cell
                std::vector<std::pair<int, int>> left_1_neighbors = get_unopened_neighbors(field, i, j);
                std::vector<std::pair<int, int>> middle_2_neighbors = get_unopened_neighbors(field, i, j + 1);
                std::vector<std::pair<int, int>> right_1_neighbors = get_unopened_neighbors(field, i, j + 2);

                // Cells above and below the middle "2" (potential mines)
                std::vector<std::pair<int, int>> middle_cells;
                int above_x = i - 1, below_x = i + 1, middle_y = j + 1;
                if (above_x >= 0 && !field.revealed[above_x][middle_y]) {
                    middle_cells.push_back({above_x, middle_y});
                }
                if (below_x < field.size && !field.revealed[below_x][middle_y]) {
                    middle_cells.push_back({below_x, middle_y});
                }

                // Find exclusive cells for left "1" (not adjacent to middle "2")
                std::vector<std::pair<int, int>> exclusive_left;
                for (const auto& cell : left_1_neighbors) {
                    bool is_exclusive = true;
                    for (const auto& middle_cell : middle_2_neighbors) {
                        if (cell.first == middle_cell.first && cell.second == middle_cell.second) {
                            is_exclusive = false;
                            break;
                        }
                    }
                    if (is_exclusive) {
                        exclusive_left.push_back(cell);
                    }
                }

                // Find exclusive cells for right "1" (not adjacent to middle "2")
                std::vector<std::pair<int, int>> exclusive_right;
                for (const auto& cell : right_1_neighbors) {
                    bool is_exclusive = true;
                    for (const auto& middle_cell : middle_2_neighbors) {
                        if (cell.first == middle_cell.first && cell.second == middle_cell.second) {
                            is_exclusive = false;
                            break;
                        }
                    }
                    if (is_exclusive) {
                        exclusive_right.push_back(cell);
                    }
                }

                // Check if the pattern is valid (e.g., middle "2" has exactly 2 unopened neighbors above/below)
                if (middle_cells.size() == 2) {
                    // Count flags around each cell to ensure no premature flagging
                    int flags_left = 0, flags_middle = 0, flags_right = 0;
                    for (const auto& cell : left_1_neighbors) {
                        if (field.flags[cell.first][cell.second]) flags_left++;
                    }
                    for (const auto& cell : middle_2_neighbors) {
                        if (field.flags[cell.first][cell.second]) flags_middle++;
                    }
					for (const auto& cell : right_1_neighbors) {
						if (field.flags[cell.first][cell.second]) flags_right++;
                    }

                    // Apply pattern if no flags interfere
                    if (flags_left == 0 && flags_middle == 0 && flags_right == 0) {
                        std::cout << "1-2-1 Pattern at (" << i << "," << j << ") to (" << i << "," << j + 2 << ")\n";

                        // Flag the cells above and below the middle "2" as mines
                        for (const auto& cell : middle_cells) {
                            int x = cell.first, y = cell.second;
                            if (!field.flags[x][y]) {
                                std::cout << "  Flagging mine at (" << x << "," << y << ")\n";
                                field.flags[x][y] = true;
                            }
                        }

                        // Reveal exclusive safe cells for left "1"
                        for (const auto& cell : exclusive_left) {
                            int x = cell.first, y = cell.second;
                            if (!field.flags[x][y]) {
                                std::cout << "  Safe to reveal: (" << x << "," << y << ")\n";
                                field.reveal_square(x, y);
                            }
                        }

                        // Reveal exclusive safe cells for right "1"
                        for (const auto& cell : exclusive_right) {
                            int x = cell.first, y = cell.second;
                            if (!field.flags[x][y]) {
                                std::cout << "  Safe to reveal: (" << x << "," << y << ")\n";
                                field.reveal_square(x, y);
                            }
                        }
                    }
                }
            }
        }
    }
}

//1-2-2-1
void pattern_1_2_2_1(Minefield& field) {
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size - 3; j++) {
            if (field.grid[i][j] == 1 && field.grid[i][j+1] == 2 &&
                field.grid[i][j+2] == 2 && field.grid[i][j+3] == 1) {
                
            }
        }
    }
}


//eventually function to call to test all patterns at once
void pattern_recognition(Minefield& field) {
    B1(field);
    B2(field);
    //pattern_1_1(field);
    //pattern_1_2(field);
    //pattern_1_2_corner(field);
    //pattern_1_2_1(field);
    //pattern_1_2_2_1(field);
}


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

//Helper function to simulate revealing and running the bot
void test_bot() {
    //5x5 grid example (mines are marked as -1)
	Minefield field("dummy", 5, 2, {0,0});

    field.grid = {
        {1,  1,  1, 0, 0},
        {1,  -1,  1, 0, 0},
        {1,  1,  1, 0, 0},
        {0,  0,  0, 0, 0},
        {0,  0,  0, 0, 0}
    };

	//Resize and initialize supporting structures
    field.revealed = std::vector<std::vector<bool>>(field.size, std::vector<bool>(field.size, false));
    field.flags = std::vector<std::vector<bool>>(field.size, std::vector<bool>(field.size, false));

	//Simulate bot revealing the 2
	field.revealed[0][0] = true;
	field.revealed[0][1] = true;
	field.revealed[1][0] = true;
    
/*	//debug prints
	std::cout << "Initial grid:\n";
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            std::cout << field.grid[i][j] << " ";
        }
        std::cout << "\n";
    }
	//prints the initals reveal calls
    std::cout << "Initial revealed state:\n";
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            std::cout << (field.revealed[i][j] ? "T " : "F ");
        }
        std::cout << "\n";
    }

	//prints whether a flag is currently marked
    std::cout << "Initial flags state:\n";
    for (int i = 0; i < field.size; i++) {
        for (int j = 0; j < field.size; j++) {
            std::cout << (field.flags[i][j] ? "T " : "F ");
        }
        std::cout << "\n";
    }

	*/

    //Output the field before running the bot
    std::cout << "Board before bot:\n";
    field.output_field();

	//run Bot
	pattern_recognition(field);


    //Output the field after the bot has run
    std::cout << "\nBoard after bot:\n";
    bot_output_field(field);
	
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
