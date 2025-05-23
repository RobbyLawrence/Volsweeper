# Volsweeper
This repository is the final project of a small group of students for Data Structures &amp; Algorithms II @ UTK.
Project Members: Robby Lawrence, Matthew Ferrari, Brett Dowling, Hien Vo

## Description:
The goal of this project is to create a playable Minesweeper (hereafter referred to as "Volsweeper) executable.
This program should open in a window, and allow the user to interact with the program through a GUI.
We're aiming to create a program that can also play the game by providing the user with the most optimal move
given a minefield. A common pitfall in Minesweeper implementations is that the player isn't guaranteed to click a good starting square. We've fixed this by generating the board after the player clicks on their first square, ensuring that square is safe, as well as all adjacent squares. Finally, we'd like to implement an algorithm that generates minefields that can be solved without
guessing.
After much deliberation, we decided that it was infeasible to implement solvable boards. The algorithm designed to handle the generation would work, but we'd require a more fleshed out bot for the algorithm to run faster than the simplest solvable board generator you can think of.

One thing that we have implemented in our minesweeper version that is different than the standard version is that we have implemented a "hint" button. What this button will do is show you the next possible safe move. We have programmed patterns off of the official
minesweeper website. The main two that we will be using are called "B1" and "B2". B1 will flag an unopened square as a mine
if the number of unopened squares is the same as the value as the cell. B2 will open all adjacent squares to the cell if the number
of flags adjacent to the cell is the same as the value of the cell.

## How to Build
Notice that there is no `build` directory for CMake. Run `mkdir build && cd build`, and then `cmake ..`.
After this, all you have to do to compile is run `cmake --build build` in the root
directory, and the binaries will compile in the `bin` folder. Each binary can be run with the `-h` flag for information
on usage.

If you need to add another header file in the `include` directory, you'll need to change `CMakeLists.txt`. This
is an involved process!. One should not do this unless they are comfortable with the CMake build system and the
processes involved therein!
