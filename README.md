# Volsweeper
This repository is the final project of a small group of students for Data Structures &amp; Algorithms II @ UTK.
Project Members: Robby Lawrence, Matthew Ferrari, Brett Dowling, Hien Vo

## Description:
The goal of this project is to create a playable Minesweeper (hereafter referred to as "Volsweeper) executable.
This program should open in a window, and allow the user to interact with the program through a GUI.
We're aiming to create a program that can also play the game by providing the user with the most optimal move
given a minefield. Finally, we'd like to implement an algorithm that generates minefields that can be solved without
guessing.

## How to Build
Notice that there is no `build` directory for CMake. Run `mkdir build && cd build`, and then `cmake ..`.
After this, all you have to do to compile is run `cmake --build build` in the root
directory, and the binaries will compile in the `bin` folder. Each binary can be run with the `-h` flag for information
on usage.

If you need to add another header file in the `include` directory, you'll need to change `CMakeLists.txt`. This
is an involved process!. One should not do this unless they are comfortable with the CMake build system and the
processes involved therein!
