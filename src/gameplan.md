# How am I going to make good boards?
I've been thinking about this a lot, and decided that I want to write my thoughts down so that I can actually begin implementing it soon.

# Method
To generate good boards (I'll be testing on 16 x 16 boards with 40 mines), we'll use a solver on a random board. When the solver gets stuck, there'll be a natural "frontier" that's formed from revealing squares. We'll then allow the perturber to take over, and it'll move a mine from unopened space to a non-mine space on the frontier.
