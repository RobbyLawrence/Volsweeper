#!/usr/bin/env python3
import sys, os


# —————————————————————————————
# 1) Make sure Python can find your compiled extension:
script_dir = os.path.dirname(os.path.abspath(__file__))
build_dir  = os.path.join(script_dir, "build")
sys.path.insert(0, build_dir)
# —————————————————————————————

import pygame
import volsweeper_binding as vb

from pgu import gui

# —————————————————————————————
# 2) Board parameters
WIDTH, HEIGHT, MINES = 10, 10, 20
cell_size = 80
# —————————————————————————————

# 3) Initialize C++ logic
vs = vb.Minefield("-r", WIDTH, MINES)

# 4) Pull initial state
grid     = vs.get_grid()      # [[int]]
revealed = vs.get_revealed()  # [[bool]]

# —————————————————————————————
# 5) Pygame setup
pygame.init()
screen = pygame.display.set_mode((WIDTH * cell_size, HEIGHT * cell_size))
pygame.display.set_caption("VolSweeper GUI")
font = pygame.font.Font(None, 24)
#

def draw_board(grid, revealed):
    """Draw the entire board: grid holds -1 or counts, revealed flags open tiles."""
    screen.fill((0, 0, 0))  # background

    for y, row in enumerate(grid):
        for x, val in enumerate(row):
            rect = pygame.Rect(x * cell_size, y * cell_size, cell_size, cell_size)

            # fill revealed tiles with a dark grey so numbers show up
            if revealed[y][x]:
                pygame.draw.rect(screen, (50, 50, 50), rect)
            # else: leave it black for unrevealed

            # draw the tile border
            pygame.draw.rect(screen, (200, 200, 200), rect, 1)

            # if it’s revealed, draw a mine or a number
            if revealed[y][x]:
                if val == -1:
                    # draw a red mine
                    pygame.draw.circle(
                        screen,
                        (255, 0, 0),
                        rect.center,
                        cell_size // 4,
                        messagebox.showinfo("goodbye", "you have hit a bomb")
                    )
                elif val > 0:
                    # draw the adjacent‑mine count in white
                    txt = font.render(str(val), True, (255, 255, 255))
                    screen.blit(txt, (rect.x + cell_size//4, rect.y + cell_size//4))

    pygame.display.flip()

# 6) Draw the starting board
draw_board(grid, revealed)

# —————————————————————————————
# 7) Main event loop: handle clicks + quit
running = True
while running:
    for e in pygame.event.get():
        if e.type == pygame.QUIT:
            running = False

        # Left‑click to reveal a tile
        elif e.type == pygame.MOUSEBUTTONDOWN and e.button == 1:
            mx, my = e.pos
            tx, ty = mx // cell_size, my // cell_size

            # a) update C++ state
            vs.reveal_square(ty, tx)

            # b) re‑fetch updated revealed grid
            revealed = vs.get_revealed()

            # c) redraw with numbers and bombs
            draw_board(grid, revealed)

pygame.quit()

