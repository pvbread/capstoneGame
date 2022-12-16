import pygame
import SpriteSheet

#set up window
pygame.init()
SCREEN_WIDTH = 500
SCREEN_HEIGHT = 500

screen = pygame.display.set_mode((SCREEN_WIDTH,SCREEN_HEIGHT))
pygame.display.set_caption('Spritesheets')

# load sprite sheet
sprite_sheet_image = pygame.image.load('doux.png').convert_alpha()
sprite_sheet=SpriteSheet.SpriteSheet(sprite_sheet_image)

#set background colors
BG = (50,50,50)
BLACK = (0,0,0)

#create animation list
animation_list =[]
animation_steps = [4,6,3,4] # each number represents number of animations to iterrate through
action = 0
# set up clock to change animation for a period of time
last_update = pygame.time.get_ticks()
animation_cooldown = 75 # in milliseconds
frame = 0
step_counter=0
# add sprites from spritesheet into the list
for animation in animation_steps:
    temp_img_list=[]
    for _ in range(animation):
        temp_img_list.append(sprite_sheet.get_image(step_counter,24,24,3, BLACK))
        step_counter+=1
    animation_list.append(temp_img_list)

# game loop
running=True
while running:
    #update background
    screen.fill(BG)
    #update animation
    current_time=pygame.time.get_ticks()
    if current_time-last_update>=animation_cooldown:
        frame +=1
        last_update = current_time
        if frame>=len(animation_list[action]):
            frame=0
    #show frame image
    screen.blit(animation_list[action][frame],(0,0))

    #event handler
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type==pygame.KEYDOWN:
            if event.key==pygame.K_DOWN and action >0:
                action -=1
                frame = 0
            if event.key==pygame.K_UP and action < len(animation_list)-1:
                action +=1
                frame = 0
    pygame.display.update()
    
pygame.quit()
