#!/usr/bin/env python3
import sys, os, time
import tkinter as tk
from tkinter import messagebox

# 1) Make sure Python can find the compiled extension
script_dir = os.path.dirname(os.path.abspath(__file__))
build_dir  = os.path.join(script_dir, "build")
sys.path.insert(0, build_dir)

import pygame
import volsweeper_binding as vb

# 2) Board & UI parameters
WIDTH, HEIGHT, MINES       = 10, 10, 20
CELL_SIZE, HEADER_HEIGHT   = 80, 30
FPS                        = 30
HINT_BUTTON_WIDTH          = 120
HINT_BUTTON_HEIGHT         = 40
HINT_BUTTON_COLOR          = (255, 165, 0)
HINT_BUTTON_TEXT           = "Hint"

# 3) Color palette
BG_COLOR         = (255,255,255)
UNREVEALED_COLOR = (200,200,200)
REVEALED_COLOR   = (240,240,240)
ACCENT_COLOR     = (255,165,0)
BORDER_COLOR     = (0,0,0)
TEXT_COLOR       = (0,0,0)
MINE_COLOR       = (255,0,0)
HINT_FLAG_COLOR  = (255,255,150)

# 4) Number colors
NUMBER_COLORS = {
    1: (0,   0,   0),    # black
    2: (0,   0, 255),    # blue
    3: (255, 0,   0),    # red
    4: (0, 255,   0),    # green
}

def load_flag_image(path="volsFlag.jpg"):
    img = pygame.image.load(path).convert_alpha()
    return pygame.transform.scale(img, (CELL_SIZE, CELL_SIZE))

def draw_board(screen, font, grid, revealed, flags, flag_img,
               elapsed_time, hint_button_rect, hinted_flags):
    screen.fill(BG_COLOR)
    # Timer
    screen.blit(font.render(f"Time: {int(elapsed_time)}s", True, TEXT_COLOR), (5,5))
    # Flag count
    ft = font.render(f"Flags: {len(flags)}/{MINES}", True, TEXT_COLOR)
    screen.blit(ft, ft.get_rect(topright=(WIDTH*CELL_SIZE-5,5)))

    # Cells
    for y in range(HEIGHT):
        for x in range(WIDTH):
            rect = pygame.Rect(x*CELL_SIZE,
                               y*CELL_SIZE + HEADER_HEIGHT,
                               CELL_SIZE, CELL_SIZE)
            color = REVEALED_COLOR if revealed[y][x] else UNREVEALED_COLOR
            pygame.draw.rect(screen, color, rect)
            # 3D border
            pygame.draw.line(screen, BG_COLOR,     rect.topleft, rect.topright,    2)
            pygame.draw.line(screen, BG_COLOR,     rect.topleft, rect.bottomleft,  2)
            pygame.draw.line(screen, ACCENT_COLOR, rect.bottomleft, rect.bottomright, 2)
            pygame.draw.line(screen, ACCENT_COLOR, rect.topright,   rect.bottomright, 2)
            pygame.draw.rect(screen, BORDER_COLOR, rect, 1)

            if revealed[y][x]:
                v = grid[y][x]
                if v == -1:
                    pygame.draw.circle(screen, MINE_COLOR, rect.center, CELL_SIZE//4)
                elif v > 0:
                    # choose custom color if defined, else default
                    col = NUMBER_COLORS.get(v, TEXT_COLOR)
                    txt = font.render(str(v), True, col)
                    screen.blit(txt, txt.get_rect(center=rect.center))
            elif (y, x) in flags:
                if (y, x) in hinted_flags:
                    pygame.draw.rect(screen, HINT_FLAG_COLOR, rect)
                screen.blit(flag_img, rect.topleft)

    # Hint button
    pygame.draw.rect(screen, HINT_BUTTON_COLOR, hint_button_rect)
    bt = font.render(HINT_BUTTON_TEXT, True, TEXT_COLOR)
    screen.blit(bt, bt.get_rect(center=hint_button_rect.center))

    pygame.display.flip()

def show_game_over_popup():
    pygame.quit()
    root = tk.Tk(); root.withdraw()
    res = messagebox.askquestion("Game Over",
                                 "You hit a mine!\nRestart or quit?",
                                 icon='warning')
    if res == 'yes':
        os.execl(sys.executable, sys.executable, *sys.argv)
    else:
        sys.exit()

def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH*CELL_SIZE,
                                      HEIGHT*CELL_SIZE + HEADER_HEIGHT))
    pygame.display.set_caption("VolSweeper GUI")
    font  = pygame.font.SysFont('Arial',24,bold=True)
    clock = pygame.time.Clock()
    flag_img = load_flag_image()

    vs           = None
    grid         = [[0]*WIDTH for _ in range(HEIGHT)]
    revealed     = [[False]*WIDTH for _ in range(HEIGHT)]
    flags        = set()
    hinted_flags = set()
    start_time   = None
    elapsed_time = 0
    game_over    = False

    hint_button_rect = pygame.Rect(
        WIDTH*CELL_SIZE - HINT_BUTTON_WIDTH - 10,
        HEIGHT*CELL_SIZE + HEADER_HEIGHT - HINT_BUTTON_HEIGHT - 5,
        HINT_BUTTON_WIDTH,
        HINT_BUTTON_HEIGHT
    )

    draw_board(screen, font, grid, revealed, flags, flag_img,
               elapsed_time, hint_button_rect, hinted_flags)

    while True:
        for e in pygame.event.get():
            if e.type == pygame.QUIT:
                pygame.quit()
                return

            if e.type == pygame.MOUSEBUTTONDOWN:
                mx, my = e.pos

                # Hint button
                if hint_button_rect.collidepoint(mx, my) and vs and not game_over:
                    hint = vs.pattern_recognition()
                    #hint = vs.B1()
                    if hint != (-1,-1):
                        r, c = hint
                        flags.add((r, c))
                        hinted_flags = {(r, c)}
                    continue

                # Board click
                cx = mx // CELL_SIZE
                cy = (my - HEADER_HEIGHT) // CELL_SIZE
                if not (0 <= cy < HEIGHT and 0 <= cx < WIDTH):
                    continue

                # ─── First click: generate & flood-fill
                if e.button == 1 and vs is None:
                    vs = vb.Minefield("-r", WIDTH, MINES, (cx+1, HEIGHT-cy))
                    vs.reveal_square(cy, cx)
                    grid     = vs.get_grid()
                    revealed = vs.get_revealed()
                    start_time = time.time()

                # ─── Subsequent clicks
                elif vs and not game_over:
                    if e.button == 1:
                        if revealed[cy][cx]:
                            vs.chord(cy, cx)
                        vs.reveal_square(cy, cx)
                        if vs.get_grid()[cy][cx] == -1:
                            game_over = True
                            show_game_over_popup()
                        revealed = vs.get_revealed()
                    elif e.button == 3:
                        vs.flag_square(cy, cx)
                        coord = (cy, cx)
                        if coord in flags:
                            flags.remove(coord)
                        else:
                            flags.add(coord)

                    grid = vs.get_grid()

        if start_time and not game_over:
            elapsed_time = time.time() - start_time

        draw_board(screen, font, grid, revealed, flags, flag_img,
                   elapsed_time, hint_button_rect, hinted_flags)
        clock.tick(FPS)

if __name__ == "__main__":
    main()

