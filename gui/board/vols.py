#!/usr/bin/env python3
import sys
import os
import time
# —————————————————————————————
# 1) Make sure Python can find your compiled extension:
script_dir = os.path.dirname(os.path.abspath(__file__))
build_dir  = os.path.join(script_dir, "build")
sys.path.insert(0, build_dir)
# —————————————————————————————

import pygame
import volsweeper_binding as vb

# —————————————————————————————
# 2) Board parameters
WIDTH, HEIGHT, MINES = 10, 10, 20
CELL_SIZE = 70
# —————————————————————————————

def load_flag_image(path="volsFlag.jpg"):
    """
    Load and scale the flag icon to CELL_SIZE.
    """
    img = pygame.image.load(path).convert_alpha()
    return pygame.transform.scale(img, (CELL_SIZE, CELL_SIZE))


def draw_board(screen, font, grid, revealed, flags, flag_img):
    """
    Render the Minesweeper board: numbers, mines, and flags.
    """
    screen.fill((0, 0, 0))  # background

    #draw timer
    timer_txt = font.render(f"time: {int(elapsed_time)}s", True, (255,255,255))
    screen.blit(timer_txt, (5,5))

    for y in range(HEIGHT):
        for x in range(WIDTH):
            rect = pygame.Rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE)
            # tile border
            pygame.draw.rect(screen, (200, 200, 200), rect, 1)

            if revealed[y][x]:
                # revealed tile background
                pygame.draw.rect(screen, (50, 50, 50), rect)
                val = grid[y][x]
                if val == -1:
                    # mine
                    pygame.draw.circle(screen, (255, 0, 0), rect.center, CELL_SIZE // 4)
                elif val > 0:
                    # adjacent mine count
                    txt = font.render(str(val), True, (255, 255, 255))
                    screen.blit(txt, txt.get_rect(center=rect.center))
            elif (y, x) in flags:
                # draw a flag
                screen.blit(flag_img, rect.topleft)

    pygame.display.flip()
 


def main():
    #pygame setup
    pygame.init()
    screen = pygame.display.set_mode((WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE))
    pygame.display.set_caption("VolSweeper GUI")
    font = pygame.font.Font(None, 24)
   
    #loading flag image
    flag_img = load_flag_image()
    
    # Initialize C++ Minefield
    vs = vb.Minefield("-r", WIDTH, MINES)
    grid = vs.get_grid()            # [[int]]
    revealed = vs.get_revealed()    # [[bool]]
    flags = set()                   #track flagged corrdinates as (row,col) 

    # Initial draw
    draw_board(screen, font, grid, revealed, flags, flag_img)

    # Main event loop
    running = True
    while running:
        for e in pygame.event.get():
            if e.type == pygame.QUIT:
                running = False

            elif e.type == pygame.MOUSEBUTTONDOWN:
                mx, my = e.pos
                col, row = mx // CELL_SIZE, my // CELL_SIZE

                if e.button == 1:
                    # left-click: reveal square
                    vs.reveal_square(row, col)
                elif e.button == 3:
                    # right-click: toggle flag
                    vs.flag_square(row, col)
                    coord = (row, col)
                    if coord in flags:
                        flags.remove(coord)
                    else:
                        flags.add(coord)

                # refresh state
                grid = vs.get_grid()
                revealed = vs.get_revealed()
                # redraw
                draw_board(screen, font, grid, revealed, flags, flag_img)

    pygame.quit()



#where main is run
if __name__ == "__main__":
    main()

