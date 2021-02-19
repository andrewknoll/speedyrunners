import pygame, sys
from pygame.locals import *

#Set up pygame
pygame.init()

#Set up the window
windowSurface = pygame.display.set_mode((500, 400), 0 , 32)
pygame.display.set_caption('sprites de esos')

#Set up the colors
BLACK = (0,0,0)
RED = (255,0,0)
GREEN = (0,255,0)
BLUE = (0,0,255)
WHITE = (255,255,255)

#Set up fonts
basicFont = pygame.font.SysFont(None, 48)

#Set up the text
text = basicFont.render('HELLO WORLD', True, WHITE)
textRect = text.get_rect()
textRect.centerx = windowSurface.get_rect().centerx
textRect.centery = windowSurface.get_rect().centery

#Draw the white background onto the surface
#
# #Draw a blue poligon onto the surface
# pygame.draw.polygon(windowSurface, BLUE, ((250, 0), (500,200),(250,400), (0,200) ))
#
# #Draw a green poligon onto the surface
# pygame.draw.polygon(windowSurface, GREEN, ((125,100),(375,100),(375,300),(125,300)))

#
# #Draw the text onto the surface
# windowSurface.blit(text,textRect)

pos = [250,200]
direccion = [1, 1]

#Run the game loop
while True:
    windowSurface.fill(BLACK)

    #Draw a red circle onto the surface
    pos[0] = pos[0]+direccion[0]
    pos[1] = pos[1]+direccion[1]
    if pos[0] + 20 > 500 or pos[0] - 20 < 0:
        direccion[0]=-direccion[0]
    if pos[1] + 20 > 400 or pos[1] - 20 < 0:
        direccion[1]=-direccion[1]

    pygame.draw.circle(windowSurface, RED, pos, 20)
    #Draw the window onto the screen
    pygame.time.wait(10)
    pygame.display.update()
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
