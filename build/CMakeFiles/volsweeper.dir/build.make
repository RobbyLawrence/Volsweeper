# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rlawren9/COSC-302/projects/Minesweeper

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rlawren9/COSC-302/projects/Minesweeper/build

# Include any dependencies generated for this target.
include CMakeFiles/volsweeper.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/volsweeper.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/volsweeper.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/volsweeper.dir/flags.make

CMakeFiles/volsweeper.dir/src/volsweeper.cpp.o: CMakeFiles/volsweeper.dir/flags.make
CMakeFiles/volsweeper.dir/src/volsweeper.cpp.o: /home/rlawren9/COSC-302/projects/Minesweeper/src/volsweeper.cpp
CMakeFiles/volsweeper.dir/src/volsweeper.cpp.o: CMakeFiles/volsweeper.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rlawren9/COSC-302/projects/Minesweeper/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/volsweeper.dir/src/volsweeper.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/volsweeper.dir/src/volsweeper.cpp.o -MF CMakeFiles/volsweeper.dir/src/volsweeper.cpp.o.d -o CMakeFiles/volsweeper.dir/src/volsweeper.cpp.o -c /home/rlawren9/COSC-302/projects/Minesweeper/src/volsweeper.cpp

CMakeFiles/volsweeper.dir/src/volsweeper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/volsweeper.dir/src/volsweeper.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rlawren9/COSC-302/projects/Minesweeper/src/volsweeper.cpp > CMakeFiles/volsweeper.dir/src/volsweeper.cpp.i

CMakeFiles/volsweeper.dir/src/volsweeper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/volsweeper.dir/src/volsweeper.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rlawren9/COSC-302/projects/Minesweeper/src/volsweeper.cpp -o CMakeFiles/volsweeper.dir/src/volsweeper.cpp.s

# Object files for target volsweeper
volsweeper_OBJECTS = \
"CMakeFiles/volsweeper.dir/src/volsweeper.cpp.o"

# External object files for target volsweeper
volsweeper_EXTERNAL_OBJECTS =

/home/rlawren9/COSC-302/projects/Minesweeper/bin/volsweeper: CMakeFiles/volsweeper.dir/src/volsweeper.cpp.o
/home/rlawren9/COSC-302/projects/Minesweeper/bin/volsweeper: CMakeFiles/volsweeper.dir/build.make
/home/rlawren9/COSC-302/projects/Minesweeper/bin/volsweeper: CMakeFiles/volsweeper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rlawren9/COSC-302/projects/Minesweeper/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/rlawren9/COSC-302/projects/Minesweeper/bin/volsweeper"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/volsweeper.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/volsweeper.dir/build: /home/rlawren9/COSC-302/projects/Minesweeper/bin/volsweeper
.PHONY : CMakeFiles/volsweeper.dir/build

CMakeFiles/volsweeper.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/volsweeper.dir/cmake_clean.cmake
.PHONY : CMakeFiles/volsweeper.dir/clean

CMakeFiles/volsweeper.dir/depend:
	cd /home/rlawren9/COSC-302/projects/Minesweeper/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rlawren9/COSC-302/projects/Minesweeper /home/rlawren9/COSC-302/projects/Minesweeper /home/rlawren9/COSC-302/projects/Minesweeper/build /home/rlawren9/COSC-302/projects/Minesweeper/build /home/rlawren9/COSC-302/projects/Minesweeper/build/CMakeFiles/volsweeper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/volsweeper.dir/depend

