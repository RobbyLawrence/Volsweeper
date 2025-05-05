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
				//if unopened neighbors equals the value of the tile mark the unopened neighbor as a mine
				if (unopened_neighbors.size() == val) {
                    mark_mines(field, unopened_neighbors);
                }
            }
        }
    }
}


void B2(Minefield& field) {

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
                // If flagged neighbors equal the value, add to candidates
                if (flagged_neighbors == val) {
                    candidates.push_back(std::pair<int, int>(i, j));
                }
            }
        }
    }

    // Second pass: Reveal safe unopened neighbors for each candidate
    for (std::vector<std::pair<int, int> >::iterator it = candidates.begin(); it != candidates.end(); ++it) {
        int i = it->first;
        int j = it->second;
        std::vector<std::pair<int, int> > unopened_neighbors = get_unopened_neighbors(field, i, j);
        for (std::vector<std::pair<int, int> >::iterator neighbor_it = unopened_neighbors.begin(); neighbor_it != unopened_neighbors.end(); ++neighbor_it) {
            int x = neighbor_it->first;
            int y = neighbor_it->second;
            if (!field.flags[x][y]) { // Only reveal if not flagged
                field.reveal_square(x, y);
            }
        }
    }
} 

//eventually function to call to test all patterns at once
void pattern_recognition(Minefield& field) {
    B1(field);
    B2(field);
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

	//print initial board
    field.output_field();

	//run Bot
	pattern_recognition(field);


    //Output the field after the bot has run
    std::cout << "\nBoard after bot:\n";
    bot_output_field(field);
	
}

