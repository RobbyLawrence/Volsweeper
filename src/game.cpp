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
    Minefield field = Minefield(argv[1],size,num_mines);
    std::cout << field.output_string();
    int x,y;
    char action;
    bool game_over = false;
    while (!game_over) {
        std::cout << "What action would you like to take? ";
        std::cin >> action;
        switch (action) {
            case 'R':
                std::cout << "What square would you like to reveal? Enter your response as \"x y\": ";
                std::cin >> x >> y;
                field.reveal_square(x+size-1,y);
                field.output_field();
                if (field.grid[x][y] == -1) {
                    std::cout << "You hit a mine!";
                    game_over = true;
                }
                break;
            case 'H':
                std::cout << "Possible actions:\n\"R\" - reveal a square\n\"F\" - flag a square\n\"H\" - view this menu\n";
                break;
            default:
                std::cout << "Unknown action. Please try again, or use the \"H\" command to view available actions.\n";
                break;
        }
    }
    return 0;
}
