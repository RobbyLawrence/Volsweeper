# Minesweeper
final project for Data Structures &amp; Algorithms II @ UTK

## How to Build
Everything is already configured with CMake. All you have to do to compile is run `cmake --build build` in the root
directory, and the binary will compile in the `bin` folder. The binary can be run from root with:
`./bin/volsweeper [FLAGS] [SIZE] [NUM_MINES]`

If you need to add another header file in the `include` directory, you'll need to change CMakeLists.txt. This
is an involved process, and will involve deleting and recreating the `build` directory. One should not do
this unless they are comfortable with CMake and the processes involved therein!
