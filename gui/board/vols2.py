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
# 2) Board & UI parameters
WIDTH, HEIGHT, MINES = 10, 10, 20
CELL_SIZE = 80
HEADER_HEIGHT = 30  # space for timer and flag count
FPS = 30           # frames per second for updates
# —————————————————————————————


def load_flag_image(path="volsFlag.jpg"):
    """
    Load and scale the flag icon to CELL_SIZE.
    """
    img = pygame.image.load(path).convert_alpha()
    return pygame.transform.scale(img, (CELL_SIZE, CELL_SIZE))


def draw_board(screen, font, grid, revealed, flags, flag_img, elapsed_time):
    """
    Render the Minesweeper board with timer and flag counter.
    """
    screen.fill((0, 0, 0))

    # Draw timer at top-left
    timer_txt = font.render(f"Time: {int(elapsed_time)}s", True, (255, 255, 255))
    screen.blit(timer_txt, (5, 5))

    # Draw flag counter at top-right
    flag_txt = font.render(f"Flags: {len(flags)}/{MINES}", True, (255, 255, 255))
    screen_width = WIDTH * CELL_SIZE
    flag_rect = flag_txt.get_rect(topright=(screen_width - 5, 5))
    screen.blit(flag_txt, flag_rect)

    # Draw tiles
    for y in range(HEIGHT):
        for x in range(WIDTH):
            rect = pygame.Rect(
                x * CELL_SIZE,
                y * CELL_SIZE + HEADER_HEIGHT,
                CELL_SIZE,
                CELL_SIZE
            )
            pygame.draw.rect(screen, (200, 200, 200), rect, 1)

            if revealed[y][x]:
                pygame.draw.rect(screen, (50, 50, 50), rect)
                val = grid[y][x]
                if val == -1:
                    pygame.draw.circle(screen, (255, 0, 0), rect.center, CELL_SIZE // 4)
                elif val > 0:
                    txt = font.render(str(val), True, (255, 255, 255))
                    screen.blit(txt, txt.get_rect(center=rect.center))
            elif (y, x) in flags:
                screen.blit(flag_img, rect.topleft)

    pygame.display.flip()


def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE + HEADER_HEIGHT))
    pygame.display.set_caption("VolSweeper GUI")
    font = pygame.font.Font(None, 24)
    clock = pygame.time.Clock()

    flag_img = load_flag_image()
    vs = vb.Minefield("-r", WIDTH, MINES)
    grid = vs.get_grid()
    revealed = vs.get_revealed()
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

                if start_time is None:
                    start_time = time.time()

                if e.button == 1:
                    vs.reveal_square(row, col)
                    if vs.get_grid()[row][col] == -1:
                        game_over = True
                elif e.button == 3 and not game_over:
                    vs.flag_square(row, col)
                    coord = (row, col)
                    if coord in flags:
                        flags.remove(coord)
                    else:
                        flags.add(coord)

                grid = vs.get_grid()
                revealed = vs.get_revealed()

        if start_time is not None and not game_over:
            elapsed_time = time.time() - start_time

        draw_board(screen, font, grid, revealed, flags, flag_img, elapsed_time)
        clock.tick(FPS)

    pygame.quit()


if __name__ == "__main__":
    main()

