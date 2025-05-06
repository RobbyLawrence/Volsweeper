#include <iostream>
#include "volsweeper.hpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "usage: ./volsweeper [FLAG] [SIZE] [NUM_MINES]\n";
        return 1;
    }
    int size;
    int num_mines;
    try {
        size = std::stoi(argv[2]);
    } catch (const std::invalid_argument& error) {
        std::cerr << "Invalid argument: " << error.what() << std::endl;
        return 1;
    } catch (const std::out_of_range& error) {
        std::cerr << "Out of range: " << error.what() << std::endl;
        return 1;
    }

    try {
        num_mines = std::stoi(argv[3]);
    } catch (const std::invalid_argument& error) {
        std::cerr << "Invalid argument: " << error.what() << std::endl;
        return 1;
    } catch (const std::out_of_range& error) {
        std::cerr << "Out of range: " << error.what() << std::endl;
        return 1;
    }
    std::string x_str = "-1";
    std::string y_str = "-1";
    int x = std::stoi(x_str);
    int y = std::stoi(y_str);
    int placed_flags = 0;
    char action;
    bool game_over = false;
    for (size_t i = 0;i<size;i++) { // print the first board
        for (size_t j = 0;j<size;j++) {
            std::cout << "X  ";
        }
        std::cout << '\n';
    }
    // collect first input
    std::cout << "What square would you like to reveal first? ";
    while (true) {
        std::cout << "Enter your response as \"x y\": ";
        std::cin >> x_str >> y_str; // error checking input
        try {
            x = std::stoi(x_str);
            y = std::stoi(y_str);
        } catch (const std::invalid_argument& error) {
            std::cerr << "Invalid coordinates.\n";
            continue;
        } catch (const std::out_of_range& error) {
            std::cerr << "Out of range.\n";
            continue;
        }
        if (size - y >= 0 && x - 1 >= 0 && size - y < size && x - 1 < size) {
            goto begingame; // used a goto here so that I don't have to worry about scope
        }
        std::cout << "Invalid coordinates. Please try again. \n";
    }
    begingame:
    Minefield field = Minefield(argv[1], size, num_mines, std::make_pair(x,y));
    field.reveal_square(size-y,x-1);
    field.debug_output_field();
    field.output_field();
    std::vector<std::pair<int,int> > b1_coord_vect;
    std::vector<std::pair<int,int> > b2_coord_vect;
    // the game functions with a big game loop.
    while (!game_over) {
        std::cout << "What action would you like to take? ";
        std::cin >> action;

        switch (action) {
            case 'R': {
                while (true) {
                    std::cout << "Enter \"x y\" to reveal: ";
                    std::cin >> x_str >> y_str;
                    try {
                        x = std::stoi(x_str);
                        y = std::stoi(y_str);
                    } catch (...) {
                        std::cerr << "Invalid coordinates; try again.\n";
                        continue;
                    }
                    if (x >= 1 && x <= size && y >= 1 && y <= size) {
                        break;  // valid coordinate
                    }
                    std::cerr << "Coordinates out of range; must be 1–" << size << ".\n";
                }
                // convert to 0-based row/col
                int row = size - y;
                int col = x - 1;
                if (field.revealed[row][col]) {
                    field.chord(row, col);
                }
                field.reveal_square(row, col);
                field.output_field();

                if (field.grid[row][col] == -1) {
                    std::cout << "You hit a mine!\n";
                    game_over = true;
                }
                break;
            }

            case 'F': {
                while (true) {
                    std::cout << "Enter \"x y\" to flag/unflag: ";
                    std::cin >> x_str >> y_str;
                    try {
                        x = std::stoi(x_str);
                        y = std::stoi(y_str);
                    } catch (...) {
                        std::cerr << "Invalid coordinates; try again.\n";
                        continue;
                    }
                    if (x >= 1 && x <= size && y >= 1 && y <= size) {
                        break;
                    }
                    std::cerr << "Coordinates out of range; must be 1–"
                    << size << ".\n";
                }
                int row = size - y, col = x - 1;
                if (field.flag_square(row, col)) {
                    placed_flags++;
                }
                field.output_field();
                break;
            }
            case 'M':
            std::cout
            << "Possible actions:\n"
            << "  P - print the board\n"
            << "  R - reveal a square\n"
            << "  F - flag/unflag a square\n"
            << "  H - give a hint"
            << "  M - show this menu\n";
            break;
            case 'P':
            field.output_field();
            break;
            case 'H':
            b1_coord_vect = B1(field);
            b2_coord_vect = B2(field);
            if (b1_coord_vect.size() > 0) {
                std::cout << "Hint: You can flag the (" << b1_coord_vect[0].second + 1 << ", " << size - b1_coord_vect[0].first << ").\n";
                b1_coord_vect.erase(b1_coord_vect.begin(),b1_coord_vect.begin() + 1);
            }
            else {
                if (b2_coord_vect.size() > 0) {
                    std::cout << "Hint: You can reveal the (" << b2_coord_vect[0].second + 1 << ", " << size - b2_coord_vect[0].first << ").\n";
                }
            }
            b1_coord_vect.clear();
            b2_coord_vect.clear();
            break;
            default:
            std::cout << "Unknown action. Type H for help.\n";
        }

        // call check_status exactly once
        if (check_status(field)) {
            game_over = true;
        }
    }
    return 0;
}
