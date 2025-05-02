# How am I going to make good boards?
I've been thinking about this a lot, and decided that I want to write my thoughts down so that I can actually begin implementing it soon.

# Method
To generate good boards (I'll be testing on 16 x 16 boards with 40 mines), we'll use a solver on a random board. When the solver gets stuck, there'll be a natural "frontier" that's formed from revealing squares. We'll then allow the perturber to take over, and it'll move a mine from unopened space to a non-mine space on the frontier.

Okay, how am I going to generate a random board around a safe space?

we'll be modifying the constructor for Minefield.

First idea:
We'll need to store the player's initially clicked square and adjacent squares as vector of pairs of integers named safe_area.
The squares in the vector of pairs will be excluded, and we'll sprinkle mines throughout this vector with the same method as before. Then, we'll have to read from this vector back into the grid. As we copy
back into the grid vector (before we assign the non-mine squares their values) we make sure to check each square to see if it matches with an pair in safe_area. okay now go implement it.


During the copy-in phase, we need to determine the
