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
std::vector<std::pair<int,int>> B1(Minefield& f) {
    std::vector<std::pair<int,int>> to_flag;

    for (int i = 0; i < int(f.size); ++i) {
      for (int j = 0; j < int(f.size); ++j) {
        if (!f.revealed[i][j] || f.grid[i][j] <= 0)
          continue;

        int clue = f.grid[i][j];
        int flagged_count = 0;
        std::vector<std::pair<int,int>> unopened;

        // look at all eight neighbors
        for (int di=-1; di<=1; ++di) {
          for (int dj=-1; dj<=1; ++dj) {
            if (di==0 && dj==0) continue;
            int ni = i+di, nj = j+dj;
            if (ni<0 || ni>=int(f.size) || nj<0 || nj>=int(f.size))
              continue;

            if (f.flagged[ni][nj]) {
              ++flagged_count;
            }
            else if (!f.revealed[ni][nj]) {
              unopened.emplace_back(ni, nj);
            }
          }
        }

        int needed = clue - flagged_count;
        if (needed > 0 && int(unopened.size()) == needed) {
          // every unopened neighbor must be a mine
          to_flag.insert(to_flag.end(),
                         unopened.begin(),
                         unopened.end());
        }
      }
    }

    return to_flag;
}

std::vector<std::pair<int,int> > B2(Minefield& field) {
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
    return revealable;
}

std::pair<int, int> pattern_recognition(Minefield& field) {
    // try B1 first
    std::vector<std::pair<int, int>> v1 = B1(field);
    if (!v1.empty()) {
        return v1[0];
    }
    // try B2 is f1 no good  
    std::vector<std::pair<int,int>> v2 = B2(field); 
    if (!v2.empty()) {
        return v2[0];
    }
    // 3) Nothing found
    return std::make_pair(-1, -1);
}

