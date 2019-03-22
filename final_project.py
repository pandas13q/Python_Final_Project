import pygame, sys, random
pygame.init()
X = 800
Y = 640

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
GREEN = (0, 155, 0)
GREY = (128, 128, 128)
screen = pygame.display.set_mode((X, Y))
pygame.display.set_caption("Pygame by Siyuan and Summer")
clock = pygame.time.Clock()
pygame.time.set_timer(pygame.USEREVENT + 1, 10000)
class Platform():
    rects = []
    def __init__(self, posx, posy):
        self.surf = pygame.image.load('WallPy.bmp').convert()
        self.rect = self.surf.get_rect(midbottom=(posx, posy))
        Platform.rects.append(self.rect)
    def draw(self):
        screen.blit(self.surf, self.rect)
    def event(self):
        pass
    
class Spike():
    def __init__(self, posx, posy, direction):#0:up 1:right 2:down 3:left
        if direction==0:
            self.surf = pygame.image.load('Spike0.bmp').convert()
        elif direction==1:
            self.surf = pygame.image.load('Spike90.bmp').convert()
        elif direction==2:
            self.surf = pygame.image.load('Spike180.bmp').convert()
        else:
            self.surf = pygame.image.load('Spike270.bmp').convert()
        self.rect = self.surf.get_rect(midbottom=(posx, posy))
        Platform.rects.append(self.rect)
    def draw(self):
        screen.blit(self.surf, self.rect)
    def event(self):
        pass
    
class Exit():
    rects = []
    def __init__(self, posx, posy):
        self.surf = pygame.surface.Surface((40, 40))
        self.rect = self.surf.get_rect(midbottom=(posx, posy))
        self.surf.fill(GREY)
        Platform.rects.append(self.rect)
    def draw(self):
        screen.blit(self.surf, self.rect)
    def event(self):
        pass
    
class Player():
    def __init__(self):
        self.jump = False
        self.left = False
        self.right = False
        self.surf = pygame.image.load('AstroRLarge.bmp').convert()
        self.rect = self.surf.get_rect(midbottom=(60,600))
        self.y_speed = 0
    def event(self):
        if self.jump:
            self.y_speed = -22
            self.jump = False
        if self.left and self.rect.left > 0:
            self.rect.centerx -= 5
        if self.right and self.rect.right < X:
            self.rect.centerx += 5
        self.rect.bottom += self.y_speed
        if self.on_ground():
            if self.y_speed >= 0:
                self.rect.bottom = Platform.rects[self.rect.collidelist(Platform.rects)].top + 1
                self.y_speed = 0
            else:
                self.rect.top = Platform.rects[self.rect.collidelist(Platform.rects)].bottom
                self.y_speed = 2
        else:
            self.y_speed += game.acc
    def on_ground(self):
        collision = self.rect.collidelist(Platform.rects)
        if collision > -1:
            return True
        else:
            return False
    def draw(self):
        screen.blit(self.surf, self.rect)
        
class Enemy():
    def __init__(self):
        self.surf = pygame.image.load('Alien0.bmp').convert()
        self.rect = self.surf.get_rect(midtop=(700,500))
        self.x_speed = random.randint(3, 7)
        self.y_speed = 0
    def event(self):
        self.rect.centerx += self.x_speed
        if self.rect.left <= 0 or self.rect.right >= X:
            self.x_speed *= -1
        if self.on_ground():
            self.rect.bottom = Platform.rects[self.rect.collidelist(Platform.rects)].top + 1
            self.y_speed = 0
        else:
            self.y_speed += game.acc
        self.rect.bottom += self.y_speed
        self.hit()
        if game.timer:
            game.timer = False
            self.rect.midtop = (X//2, 0)
            self.x_speed = random.randint(3, 7) * ((self.x_speed > 0) - (self.x_speed < 0))

    def on_ground(self):
        collision = self.rect.collidelist(Platform.rects)
        if collision > -1:
            return True
        else:
            return False
    def hit(self):
        if (game.player.rect.colliderect(self.rect) and
            game.player.rect.midbottom != (60, 600 )):
            game.lives -= 1
            game.player.rect.midbottom = (60, 600)
            self.rect.midbottom = (420, 140)
    def draw(self):
        screen.blit(self.surf, self.rect)

class Coin():
    def __init__(self):
        self.positions = [(460,440), (280,480), (60, 600), (700, 600), (540, 320), (60,120), (140, 120)]
        self.surf = pygame.image.load('coin0.bmp').convert()
        self.rect = self.surf.get_rect(midbottom=random.choice(self.positions))
        self.count = 0
    def event(self):
        if game.player.rect.colliderect(self.rect):
            self.rect.midbottom = random.choice(self.positions)
            game.coin_count += 1
        elif game.enemy.rect.colliderect(self.rect):
            self.rect.midbottom = random.choice(self.positions)
    def draw(self):
        screen.blit(self.surf, self.rect)
        
class Game():
    def __init__(self):
        self.heart_surf = pygame.image.load('SpriteAstroR0.bmp').convert()
        self.coin_surf = pygame.transform.scale(pygame.image.load('coin0.bmp').convert(), (20, 20))
        self.acc = 2
        self.timer = False
        self.level = 0
        self.state = self.startpage
    def init(self):
        self.lives = 5
        self.coin_count = 0
        self.sprites = [self]
        self.state = self.loop
        self.player = Player()
        self.sprites.append(self.player)
        self.enemy = Enemy()
        self.sprites.append(self.enemy)
        self.coin = Coin()
        self.sprites.append(self.coin)
        
        for i in range(20):
            self.sprites.append(Platform(20+i*40,640))
        for i in range(13):
            self.sprites.append(Platform(20+i*40, 160))
        for i in range(4):
            self.sprites.append(Platform(500+i*40,360))
            self.sprites.append(Platform(580+i*40, 240))
        for i in range(6):
            self.sprites.append(Platform(120+i*40,520))
        for i in  range (7):
            self.sprites.append(Platform(420+i*40,480))
            self.sprites.append(Platform(20+i*40,360))
        
    
    def event(self):
        " a game state function "
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE and self.player.on_ground():
                    self.player.jump = True
            elif event.type == pygame.USEREVENT + 1:
                self.timer = True
        self.player.left = False
        self.player.right = False
        keys = pygame.key.get_pressed()
        if keys[pygame.K_LEFT]:
            self.player.left = True
        if keys[pygame.K_RIGHT]:
            self.player.right = True
    def draw(self):
        for i in range(self.lives):
            screen.blit(self.heart_surf, [i*20 + 20, 20])
        for i in range(self.coin_count):
            screen.blit(self.coin_surf, [750 - i*20, 20])

    def loop(self):
        " a game state function "
        clock.tick(30)
        screen.fill(GREY)
        for s in self.sprites:
            s.event()
        for s in self.sprites:
            s.draw()
        pygame.display.flip()
        if self.lives == 0:
            self.state = self.play_again_lose
        if self.coin_count == 10:#######################level up condition
            self.state = self.play_again_win
    def play_again_win(self):
        " a game state function "
        font = pygame.font.SysFont("Segoe Print", 30)
        txt_surf = font.render("You Win!", 1, GREEN)
        txt_rect = txt_surf.get_rect(center=(X//2, Y//2))
        screen.blit(txt_surf, txt_rect)
        play = button("Play Again", GREEN, ((X-200)//3, Y//8*5))
        stop = button("Stop", RED, ((X-200)//3*2+100, Y//8*5))
        pygame.display.flip()
        self.buttons = [(play, self.init), (stop, self.endpage)]
        self.state = self.mouse_click
    def play_again_lose(self):
        " a game state function "
        font = pygame.font.SysFont("Segoe Print", 30)
        txt_surf = font.render("You Lose...", 1, RED)
        txt_rect = txt_surf.get_rect(center=(X//2, Y//2))
        screen.blit(txt_surf, txt_rect)
        play = button("Play Again", GREEN, ((X-200)//3, Y//8*5))
        stop = button("Stop", RED, ((X-200)//3*2+100, Y//8*5))
        pygame.display.flip()
        self.buttons = [(play, self.init), (stop, self.endpage)]
        self.state = self.mouse_click
    def mouse_click(self):
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.state = self.end
                    return
                elif event.type == pygame.MOUSEBUTTONDOWN and pygame.mouse.get_pressed() == (1, 0, 0):
                    pos = pygame.mouse.get_pos()
                    for btn, action in self.buttons:
                        if btn.collidepoint(pos):
                            self.state = action
                            return
            pygame.time.wait(20)
    def end(self):
        pygame.quit()
        sys.exit()
    def startpage(self):
        " a game state function "
        screen.fill(GREY)
        font = pygame.font.SysFont("Segoe Print", 30)
        txt_surf = font.render("The Last Astronaut", 1, BLUE)
        txt_rect = txt_surf.get_rect(center=(X//2, Y//2))
        screen.blit(txt_surf, txt_rect)
        play = button("Play Now", BLACK, ((X-100)//2, Y//8*5))
        self.buttons = [(play, self.init)]
        pygame.display.flip()
        self.state = self.mouse_click
    def endpage(self):
        " a game state function "
        screen.fill(GREY)
        font = pygame.font.SysFont("Segoe Print", 30)
        txt_surf = font.render("Thank you for playing", 1, RED)
        txt_rect = txt_surf.get_rect(center=(X//2, Y//2))
        screen.blit(txt_surf, txt_rect)
        stop = button("Exit", BLACK, ((X-100)//2, Y//8*5))
        self.buttons = [(stop, self.end)]
        pygame.display.flip()
        self.state = self.mouse_click
def button(txt, color, pos):
    button_font = pygame.font.SysFont("Segoe Print", 16)
    btn_surf = pygame.Surface((100, 40))
    btn_rect = btn_surf.get_rect(topleft=(pos))
    btn_surf.fill(GREY)
    pygame.draw.rect(btn_surf, BLACK, (0, 0, 100, 40), 1)
    txt_surf = button_font.render(txt, 1, color)
    txt_rect = txt_surf.get_rect(center=(50,20))
    btn_surf.blit(txt_surf, txt_rect)
    screen.blit(btn_surf, btn_rect)
    return btn_rect

game = Game()

while True:
    game.state()