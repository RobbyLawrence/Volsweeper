#!/usr/bin/env python3
import sys
import os
import time
import tkinter as tk
from tkinter import messagebox

# —————————————————————————————
# 1) Make sure Python can find your compiled extension:
script_dir = os.path.dirname(os.path.abspath(__file__))
build_dir  = os.path.join(script_dir, "build")
sys.path.insert(0, build_dir)
# —————————————————————————————

import pygame
import volsweeper_binding as vb

# —————————————————————————————
# 2) Board & UI parameters
WIDTH, HEIGHT, MINES = 10, 10, 20
CELL_SIZE = 80
HEADER_HEIGHT = 30  # space for timer and flag count
FPS = 30            # frames per second for updates
# —————————————————————————————

# —————————————————————————————
# 3) Color palette
BG_COLOR         = (255, 255, 255)  # white background
UNREVEALED_COLOR = (200, 200, 200)  # light grey for covered tiles
REVEALED_COLOR   = (240, 240, 240)  # very light grey for opened tiles
BORDER_COLOR     = (0, 0, 0)        # black border
ACCENT_COLOR     = (255, 165, 0)    # orange accent
TEXT_COLOR       = (0, 0, 0)        # black text/numbers
MINE_COLOR       = (255, 0, 0)      # red mines

# —————————————————————————————


def load_flag_image(path="volsFlag.jpg"):
    img = pygame.image.load(path).convert_alpha()
    return pygame.transform.scale(img, (CELL_SIZE, CELL_SIZE))


def draw_board(screen, font, grid, revealed, flags, flag_img, elapsed_time):
    screen.fill(BG_COLOR)

    timer_txt = font.render(f"Time: {int(elapsed_time)}s", True, TEXT_COLOR)
    screen.blit(timer_txt, (5, 5))

    flag_txt = font.render(f"Flags: {len(flags)}/{MINES}", True, TEXT_COLOR)
    screen_width = WIDTH * CELL_SIZE
    flag_rect = flag_txt.get_rect(topright=(screen_width - 5, 5))
    screen.blit(flag_txt, flag_rect)

    for y in range(HEIGHT):
        for x in range(WIDTH):
            rect = pygame.Rect(x * CELL_SIZE, y * CELL_SIZE + HEADER_HEIGHT, CELL_SIZE, CELL_SIZE)
            color = REVEALED_COLOR if revealed[y][x] else UNREVEALED_COLOR
            pygame.draw.rect(screen, color, rect)
            pygame.draw.line(screen, BG_COLOR, rect.topleft, rect.topright, 2)
            pygame.draw.line(screen, BG_COLOR, rect.topleft, rect.bottomleft, 2)
            pygame.draw.line(screen, ACCENT_COLOR, rect.bottomleft, rect.bottomright, 2)
            pygame.draw.line(screen, ACCENT_COLOR, rect.topright, rect.bottomright, 2)
            pygame.draw.rect(screen, BORDER_COLOR, rect, 1)

            if revealed[y][x]:
                val = grid[y][x]
                if val == -1:
                    pygame.draw.circle(screen, MINE_COLOR, rect.center, CELL_SIZE // 4)
                elif val > 0:
                    txt = font.render(str(val), True, TEXT_COLOR)
                    screen.blit(txt, txt.get_rect(center=rect.center))
            elif (y, x) in flags:
                screen.blit(flag_img, rect.topleft)

    pygame.display.flip()


def show_game_over_popup():
    pygame.quit() #shutdown pygame so tinker can work

    root = tk.Tk()
    root.withdraw()
    result = messagebox.askquestion("Game Over", "You hit a mine!\nDo you want to restart the game?", icon='warning')
    if result == 'yes':
        os.execl(sys.executable, sys.executable, *sys.argv)
    else:
        sys.exit()


def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE + HEADER_HEIGHT))
    pygame.display.set_caption("VolSweeper GUI")
    font = pygame.font.SysFont('Arial', 24, bold=True)
    clock = pygame.time.Clock()

    flag_img = load_flag_image()

    vs = None
    grid = [[0] * WIDTH for _ in range(HEIGHT)]
    revealed = [[False] * WIDTH for _ in range(HEIGHT)]
    flags = set()
    start_time = None
    elapsed_time = 0
    game_over = False

    draw_board(screen, font, grid, revealed, flags, flag_img, elapsed_time)

    running = True
    while running:
        for e in pygame.event.get():
            if e.type == pygame.QUIT:
                running = False

            elif e.type == pygame.MOUSEBUTTONDOWN:
                mx, my = e.pos
                col, row = mx // CELL_SIZE, (my - HEADER_HEIGHT) // CELL_SIZE
                if row < 0 or row >= HEIGHT:
                    continue

                if e.button == 1 and vs is None:
                    vs = vb.Minefield("-r", WIDTH, MINES, (row, col))
                    grid = vs.get_grid()
                    revealed = vs.get_revealed()
                    if revealed[row][col]:
                        vs.chord(row, col)
                    vs.reveal_square(row, col)
                    revealed = vs.get_revealed()
                    start_time = time.time()
                elif vs and not game_over:
                    if e.button == 1:
                        if revealed[row][col]:
                            vs.chord(row, col)
                        vs.reveal_square(row, col)
                        if vs.get_grid()[row][col] == -1:
                            game_over = True
                            show_game_over_popup()
                        revealed = vs.get_revealed()
                    elif e.button == 3:
                        vs.flag_square(row, col)
                        coord = (row, col)
                        if coord in flags:
                            flags.remove(coord)
                        else:
                            flags.add(coord)

                if vs:
                    grid = vs.get_grid()

        if start_time and not game_over:
            elapsed_time = time.time() - start_time

        draw_board(screen, font, grid, revealed, flags, flag_img, elapsed_time)
        clock.tick(FPS)

    pygame.quit()


if __name__ == "__main__":
    main()

