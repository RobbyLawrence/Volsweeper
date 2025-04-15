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
    std::cout << "What square would you like to check? Enter your answer as \"x y\": ";
    int x,y;
    std::cin >> x >> y;
    field.reveal_square(x,y);
    field.output_field();
    return 0;
}
