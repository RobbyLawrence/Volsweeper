#pygame script that loads the extension

import sys, os

#find the directory of the script
script_dir = os.path.dirname(os.path.abspath(__file__))

#Prepend the build folder so python can see the compiled extension:
build_dir = os.path.join(script_dir, "build")
sys.path.insert(0,build_dir)

import pygame
import volsweeper_binding as vb
#print("what's in the volsweeper binding", dir(vb))

# parameters
WIDTH, HEIGHT, MINES = 10, 10, 15
cell_size = 40

# initialize C++ board
vs = vb.Minefield("-r",WIDTH, HEIGHT)
grid = vs.get_grid()   # a List[List[int]]

# pygame setup
pygame.init()
screen = pygame.display.set_mode((WIDTH*cell_size, HEIGHT*cell_size))
pygame.display.set_caption("VolSweeper GUI")

# draw board once
font = pygame.font.Font(None, 24)
for y, row in enumerate(grid):
    for x, val in enumerate(row):
        rect = pygame.Rect(x*cell_size, y*cell_size, cell_size, cell_size)
        pygame.draw.rect(screen, (220,220,220), rect, 1)
        if val >= 0:
            txt = font.render(str(val), True, (0,0,0))
            screen.blit(txt, (x*cell_size+10, y*cell_size+10))
pygame.display.flip()

# event loop
running = True
while running:
    for e in pygame.event.get():
        if e.type == pygame.QUIT:
            running = False
pygame.quit()

