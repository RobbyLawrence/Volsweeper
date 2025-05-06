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
			int nx = i + x;
			int ny = j + y;
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
		std::cout << "Bot flagging mine at (" << x << "," << y << ")\n";
		field.flagged[x][y] = true; // Mark as mine
	}
}

// B1 (If the number of the square is the same as the adjacent squares then all of the squares are mines)
std::pair<int, int> B1(Minefield& field) {
	for (int i = 0; i < field.size; i++) {
		for (int j = 0; j < field.size; j++) {
			int val = field.grid[i][j];
			if (val > 0 && field.revealed[i][j]) {
				std::vector<std::pair<int, int> > unopened_neighbors = get_unopened_neighbors(field, i, j);

				if ((int)unopened_neighbors.size() == val) {
					for (size_t k = 0; k < unopened_neighbors.size(); k++) {
						int x = unopened_neighbors[k].first;
						int y = unopened_neighbors[k].second;

						if (!field.flagged[x][y]) {
							field.flagged[x][y] = true;
							return std::make_pair(x, y);  // Return immediately when found
						}
					}
				}
			}
		}
	}
	return std::make_pair(-1, -1);  // No hint found
}

/*std::vector<std::pair<int,int> > B1(Minefield& f) {
  std::vector<std::pair<int,int>> to_flag;

  for (int i = 0; i < f.size; ++i) {
  for (int j = 0; j < f.size; ++j) {
  if (!f.revealed[i][j] || f.grid[i][j] <= 0)
  continue;

// collect all *unrevealed & unflagged* neighbors
std::vector<std::pair<int,int>> cand;
for (int di = -1; di <= 1; ++di) {
for (int dj = -1; dj <= 1; ++dj) {
if (di==0 && dj==0) continue;
int ni = i+di, nj = j+dj;
if (ni>=0 && ni<f.size && nj>=0 && nj<f.size
&& !f.revealed[ni][nj] && !f.flagged[ni][nj]) {
cand.emplace_back(ni,nj);
}
}
}
// if exactly grid[i][j] of these remain, they must all be mines
if ((int)cand.size() == f.grid[i][j]) {
to_flag.insert(to_flag.end(), cand.begin(), cand.end());
}
}
}

return to_flag;
}
*/
std::pair<int,int>  B2(Minefield& field) {
	// Vector to store candidates
	std::vector<std::pair<int, int> > candidates;
	std::vector<std::pair<int, int> > revealable;
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
						if (nx >= 0 && nx < field.size && ny >= 0 && ny < field.size && field.flagged[nx][ny]) {
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
			if (!field.flagged[x][y]) { // Only reveal if not flagged
				revealable.push_back(std::make_pair(x,y));
			}
		}
	}   
	if (!revealable.empty()) {
		return revealable[0];
	} else {
		return std::make_pair(-1, -1);
	}

}

//eventually function to call to test all patterns at once
/*void pattern_recognition(Minefield& field) {
  B1(field);
  B2(field);
  }
  */
std::pair<int, int> pattern_recognition(Minefield& field) {
	std::pair<int, int> result = B1(field);
	if (result != std::make_pair(-1, -1)) return result;
	result = B2(field);
	if (result != std::make_pair(-1, -1)) return result;
	return {-1, -1};
}


//bot output
void bot_output_field(Minefield& field) {
	for (int i = 0; i < field.size; i++) {
		for (int j = 0; j < field.size; j++) {
			if (field.flagged[i][j]) {
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
	field.flagged = std::vector<std::vector<bool>>(field.size, std::vector<bool>(field.size, false));

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
