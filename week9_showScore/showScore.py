import pgzrun

WIDTH = 350
HEIGHT = 600

backgroud = Actor("background")
bird = Actor("bird")
bird.x = 50
bird.y = HEIGHT / 2
bar_up = Actor("bar_up")
bar_up.x = 300
bar_up.y = 0
bar_down = Actor("bar_down")
bar_down.x = 300
bar_down.y = 600
score = 0
speed = 1


def draw():
    backgroud.draw()
    bar_up.draw()
    bar_down.draw()
    bird.draw()
    if bird.colliderect(bar_up) or bird.colliderect(bar_down):
        print("游戏失败")
        exit()
    screen.draw.text(str(score), (30, 30), fontsize=50, color="green")


def update():
    global score, speed
    if bar_up.x < 0:
        score += 1
        bar_up.x = WIDTH
        bar_down.x = WIDTH
    speed = score // 5 + 2
    bird.y += 2
    bar_up.x -= speed
    bar_down.x -= speed


def on_mouse_down():
    bird.y -= 50


pgzrun.go()
