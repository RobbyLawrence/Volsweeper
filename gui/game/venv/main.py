import pygame

# Initialize PyGame
pygame.init()

# Create a window (800x600 pixels)
screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption("Simple Board Game")

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Left click
                print("Left click!")
            elif event.button == 3:  # Right click
                print("Right click!")
    
    # Fill the screen with white
    screen.fill((255, 255, 255))
    
    # Update the display
    pygame.display.flip()

pygame.quit()

