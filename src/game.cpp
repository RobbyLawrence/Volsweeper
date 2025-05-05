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
    int x,y = -1;
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
        std::cin >> x >> y; // error checking input
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
    while (!game_over) {
        std::cout << "What action would you like to take? ";
        std::cin >> action;
        switch (action) {
            case 'R':
            std::cout << "What square would you like to reveal? ";
            while (true) {
                std::cout << "Enter your response as \"x y\": ";
                std::cin >> x >> y; // error checking input
                if (size - y >= 0 && x - 1 >= 0 && size - y < size && x - 1 < size) {

                    break;
                }
                std::cout << "Invalid coordinates. Please try again. \n";
            }
            field.reveal_square(size-y,x-1);
            field.output_field();
            if (field.grid[size-y][x-1] == -1) {
                std::cout << "You hit a mine!\n";
                game_over = true;
            }
            for (std::vector<bool> vect : field.revealed) {
                for (bool element : vect) {
                    if (element == false) {
                        // i know goto is a bad practice, but i didn't want to use another bool
                        goto endloops;
                        // here, this works as a two-layer break
                    }
                }
            }
            // we'll only ever arrive here if everything in field.revealed is true
            std::cout << "You won!";
            game_over = true;
            endloops:
            break;
            case 'H':
            std::cout << "Possible actions:\n\"R\" - reveal a square\n\"F\" - flag/unflag a square\n\"H\" - view this menu\n";
            break;
            case 'F':
            std::cout << "What square would you like to flag? ";
            while (true) {
                std::cout << "Enter your response as \"x y\": ";
                std::cin >> x >> y; // error checking input
                if (size - y >= 0 && x - 1 >= 0 && size - y < size && x - 1 < size) {
                    break;
                }
                std::cout << "Invalid coordinates. Please try again. \n";
            }
            field.flag_square(size-y,x-1);
            placed_flags++;
            field.output_field();
            break;
            default:
            std::cout << "Unknown action. Please try again, or use the \"H\" command to view available actions.\n";
            break;
        }
        if (check_status(field)) {
            std::cout << "You won the game!\n";
            break; // this ends the game loop
        }
        else if (!check_status(field) && placed_flags == num_mines) {
            std::cout << "Your solution is incorrect!\n";
            break;
        }
    }
    return 0;
}
