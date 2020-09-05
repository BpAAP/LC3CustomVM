import pygame
from pygame.locals import *

import socket

pygame.init()

#32 by 32 position display
size = width,height = 512,512
#each 'virtual pixel' is 16 actual pixes
pixel_scaling = 16
#These pixels can be a solid color, or an ascii character

font = pygame.font.SysFont(name='arial',size=pixel_scaling)

screen = pygame.display.set_mode(size)

screen.fill((0,0,0))


running = True

def colorPixel(x,y,color):
    try:
        pygame.draw.rect(screen,color,(x*pixel_scaling,y*pixel_scaling,pixel_scaling,pixel_scaling))
    except:
        print("Something went wrong in 'colorPixel()'")
        
def writeCharacter(x,y,char,color):
    textSurface = font.render(char,True,color)
    textRectangle = textSurface.get_rect()
    textRectangle.center = (int((x+0.5)*pixel_scaling),int((y+0.5)*pixel_scaling))
    screen.blit(textSurface, textRectangle)
    
def interpret(partA,partB):
    partA = int.from_bytes(partA,byteorder="big",signed=False)
    partB = int.from_bytes(partB,byteorder="big",signed=False)
    print(partA,partB)
    if (partA>>13)==1 & (partA%256)>>5==1:
        #This is a pixel colouring command
        x = (partA >> 8)%32
        y = (partA%32)
        red = (partB>>8)*16
        green = ((partB%256)>>4)*16
        blue =  (partB%16)*16
        print("Got command: colour pixel (",x,y,") to (",red,green,blue,")")
        colorPixel(x,y,(red,green,blue,255))
        return True
    elif (partA>>13)==2 & (partA%256)>>5==2:
        #Write character
        x = (partA >> 8)%32
        y = (partA%32)
        colours = partB>>8
        red = (colours>>4)*64
        green = ((colours%16)>>2)*64
        blue = (colours%4)*64
        character = chr(partB%256)
        print("Got command: write character to pixel (",x,y,") to (",red,green,blue,") as a/an",character)
        writeCharacter(x,y,character,(red,green,blue,255))
        return True
    elif (partA>>13)==0 & (partA%256)>>5==0:
        
        print("Got null")
        return False
        


#Now for creating the server-side socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#Bind to 25560 on localhost
s.bind(('127.0.0.1',25560))
s.listen(2)

def receive():
    clientsocket, address = s.accept()
    running = True
    while running:
        for event in pygame.event.get():
            if event.type==QUIT:
                pygame.quit()
                sys.exit()
    
        partA = clientsocket.recv(2)
        partB = clientsocket.recv(2)
        
    
        running = interpret(partA,partB)
        pygame.display.update()

while True:
    receive()
    print("No longer running")
    print("Restart to start activate")
    _ = input("Press Enter to reactivate")

