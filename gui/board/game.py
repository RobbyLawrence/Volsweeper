import pygame
import sys
import os
import tkinter as tk
from tkinter import messagebox

# —————————————————————————————
# 1) Make sure Python can find your compiled extension:
script_dir = os.path.dirname(os.path.abspath(__file__))
build_dir  = os.path.join(script_dir, "build")
sys.path.insert(0, build_dir)
# —————————————————————————————

import volsweeper_binding as vb

# Constants
WIDTH, HEIGHT, MINES = 10, 10, 15
CELL_SIZE = 40


def show_exit_popup(title="Game Over", message="Thanks for playing!"):
    root = tk.Tk()
    root.withdraw()
    messagebox.showinfo(title, message)
    root.destroy()
    pygame.quit()
    sys.exit()


def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE))
    pygame.display.set_caption("Minesweeper")

    # Create Minefield: pass flag string as first argument
    vs = vb.Minefield("-r", WIDTH, MINES)
    first_click = True

    # Initialize board state for drawing
    grid = vs.get_grid()
    revealed = vs.get_revealed()

    running = True
    while running:
        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                show_exit_popup("Goodbye!", "You closed the window.")

            elif ev.type == pygame.KEYDOWN and ev.key == pygame.K_ESCAPE:
                show_exit_popup("Exited", "Escape pressed—exiting the game.")

            elif ev.type == pygame.MOUSEBUTTONDOWN and ev.button == 1:
                mx, my = ev.pos
                col, row = mx // CELL_SIZE, my // CELL_SIZE

                if first_click:
                    # Generate mines avoiding the first-click zone
                    vs.generate(row, col)
                    first_click = False

                # Reveal the clicked square (and flood-fill zeros)
                vs.reveal_square(row, col)
                grid = vs.get_grid()
                revealed = vs.get_revealed()

                # If user hit a mine, end game
                if grid[row][col] == -1:
                    show_exit_popup("Game Over", "You hit a mine!")

        # Draw board
        screen.fill((192, 192, 192))
        font = pygame.font.SysFont(None, 24)
        for r in range(HEIGHT):
            for c in range(WIDTH):
                rect = pygame.Rect(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE)
                pygame.draw.rect(screen, (128, 128, 128), rect, 1)
                if revealed[r][c]:
                    val = grid[r][c]
                    if val > 0:
                        txt = font.render(str(val), True, (0, 0, 0))
                        screen.blit(txt, txt.get_rect(center=rect.center))
                    elif val == -1:
                        pygame.draw.circle(screen, (0, 0, 0), rect.center, CELL_SIZE // 4)

        pygame.display.flip()


if __name__ == "__main__":
    main()

