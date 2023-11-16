import pgzrun  # 导入游戏库
import random  # 导入随机库
from abc import abstractmethod, ABC
import math

WIDTH = 480  # 设置窗口的宽度
HEIGHT = 700  # 设置窗口的高度
TITLE = "Python Game"
score = 0  # 游戏得分
isLoose = False  # 游戏是否失败，初始不失败
# sounds.game_music.play(-1)  # 循环播放背景音乐


# interface of all kinds of actors
class AllActors(ABC):
    actors = []

    # draw
    @abstractmethod
    def draw(self):
        pass

    # update
    @abstractmethod
    def update(self):
        pass

    @abstractmethod
    def get_actor(self):
        pass


class BackGround(AllActors):
    WIDTH = WIDTH  # 设置窗口的宽度
    HEIGHT = HEIGHT  # 设置窗口的高度
    TITLE = TITLE
    background1 = Actor("background")  # 导入背景1图片
    background1.x = 480 / 2  # 背景1的x坐标
    background1.y = 852 / 2  # 背景1的y坐标
    background2 = Actor("background")  # 导入背景2图片
    background2.x = 480 / 2  # 背景2的x坐标
    background2.y = -852 / 2  # 背景2的y坐标
    deviation = -20  # 偏差值，子弹出边界销毁


class Start_Button(AllActors):
    start = False

    def __init__(self) -> None:
        self.start_no = Actor("start_no")
        self.start_no.x = WIDTH / 2
        self.start_no.y = HEIGHT / 2
        self.start_yes = Actor("start_yes")
        self.start_yes.x = WIDTH / 2
        self.start_yes.y = HEIGHT / 2
        self.start_pic = self.start_no

    def draw(self):
        self.start_pic.draw()

    def update(self):
        if keyboard.RETURN and self.start_pic == self.start_no:
            self.start_pic = self.start_yes
        elif not keyboard.RETURN and self.start_pic == self.start_yes:
            Start_Button.start = True

    def get_actor(self):
        return self.start_pic


# super class for all kinds of bullets
class Bullets(AllActors):
    def check_collision(actor):
        pass


class Basic_Bullets(Bullets):
    def __init__(
        self, shoot_pos: tuple, v: tuple = (0, 4), target_pos: tuple = None
    ) -> None:
        """Basic_Bullets init

        Args:
            shoot_pos (tuple): bullets position
            v (tuple, optional): bullets v. Defaults to (0, 4).
            target_pos (tuple, optional): target position. Defaults to None.
        """
        self.bullet = Actor("bullet")
        self.bullet.x = shoot_pos[0]
        self.bullet.y = shoot_pos[1]
        self.bullet.vx = v[0]
        self.bullet.vy = v[1]
        self.exsit = True  # determine whether the bullet is exsit
        if target_pos is not None:
            self.bullet.angle = self.bullet.angle_to(target_pos)

    def draw(self):
        self.bullet.draw()

    def update(self):
        self.bullet.x += self.bullet.vx
        self.bullet.y += self.bullet.vy
        self.check_boundary()

    def get_actor(self):
        return self.bullet

    def check_collision(self, actor):
        self.bullet.colliderect(actor)

    def check_boundary(self):
        # out of boundary
        if (
            self.bullet.x < -BackGround.deviation
            or self.bullet.x > BackGround.WIDTH + BackGround.deviation
            or self.bullet.y > BackGround.HEIGHT + BackGround.deviation
            or self.bullet.y < -BackGround.deviation
        ):
            self.exsit = False


# player
class Hero(AllActors):
    def __init__(self) -> None:
        self.hero = Actor("hero")
        self.hero.x = BackGround.WIDTH / 2
        self.hero.y = BackGround.HEIGHT * 2 / 3
        self.hero.speed = 3
        self.bullets = []
        self.bullet_class = Basic_Bullets

    def draw(self):
        self.hero.draw()

    def update(self):
        self.key_down()

    def get_actor(self) -> Actor:
        return self.hero

    def key_down(self):  # When key down
        if keyboard.UP:
            self.hero.y -= self.hero.speed
            if keyboard.LEFT:
                self.hero.x -= self.hero.speed
                self.hero.angle = 45
            elif keyboard.RIGHT:
                self.hero.x += self.hero.speed
                self.hero.angle = 315
            else:
                self.hero.angle = 0
        elif keyboard.DOWN:
            self.hero.y += self.hero.speed
            if keyboard.LEFT:
                self.hero.x -= self.hero.speed
                self.hero.angle = 135
            elif keyboard.RIGHT:
                self.hero.x += self.hero.speed
                self.hero.angle = 225
            else:
                self.hero.angle = 180
        elif keyboard.LEFT:
            self.hero.x -= self.hero.speed
            self.hero.angle = 90
        elif keyboard.RIGHT:
            self.hero.x += self.hero.speed
            self.hero.angle = 270
        else:
            distance = 99999
            nearest = None
            for actor in AllActors.actors:
                if (
                    actor.get_actor().image != "hero"
                    and self.hero.distance_to(actor.get_actor()) < distance
                ):
                    distance = self.hero.distance_to(actor.get_actor())
                    nearest = actor
            self.hero.angle = self.hero.angle_to(nearest.get_actor()) - 90
            # shoot when the player dosen't move
            self.attack()
        self.check_boundary()

    def attack(self):  # shoot
        return
        distance = self.hero.height / 3 * 2
        pos = distance * math.sin(distance)
        # TODO:
        self.bullets.append(self.bullet_class())

    def check_boundary(self):
        if self.hero.left < 0:
            self.hero.left = 0
        if self.hero.right > BackGround.WIDTH:
            self.hero.right = BackGround.WIDTH
        if self.hero.bottom > BackGround.HEIGHT:
            self.hero.bottom = BackGround.HEIGHT
        if self.hero.top < 0:
            self.hero.top = 0


hero = Hero()


class Enemy(AllActors):
    def attack():
        pass


class Basic_Enemy(Enemy):
    def __init__(self) -> None:
        self.enemy = Actor("enemy")
        self.enemy.x = BackGround.WIDTH / 2
        self.enemy.y = BackGround.HEIGHT / 4

    def draw(self):
        self.enemy.draw()

    def update(self):
        # enemy towards the player
        for actor in AllActors.actors:
            if actor.get_actor().image == "hero":
                self.enemy.angle = self.enemy.angle_to(actor.get_actor()) + 90
                break

    def get_actor(self):
        return self.enemy

    def attack():
        pass


start_button = Start_Button()
AllActors.actors.append(hero)
AllActors.actors.append(Basic_Enemy())


def draw():  # 绘制模块，每帧重复执行
    global start, start_pic
    BackGround.background1.draw()  # 绘制游戏背景
    BackGround.background2.draw()  # 绘制游戏背景
    for actor in AllActors.actors:
        actor.draw()
    # 下面显示得分
    screen.draw.text(
        "得分: " + str(score),
        (200, BackGround.HEIGHT - 50),
        fontsize=30,
        fontname="s",
        color="black",
    )
    if isLoose:  # 游戏失败后输出信息
        screen.draw.text(
            "游戏失败！", (50, BackGround.HEIGHT / 2), fontsize=90, fontname="s", color="red"
        )
    if not Start_Button.start:
        start_button.draw()


def update():  # 更新模块，每帧重复操作
    global score, isLoose, bullets, start
    if not Start_Button.start:
        start_button.update()
        return
    if isLoose:
        return  # 如果游戏失败，返回，不做下面的操作
    for actor in AllActors.actors:
        actor.update()
    # 以下代码用于实现背景图片的循环滚动效果
    if BackGround.background1.y > 852 / 2 + 852:
        BackGround.background1.y = -852 / 2  # 背景1移动到背景2的正上方
    if BackGround.background2.y > 852 / 2 + 852:
        BackGround.background2.y = -852 / 2  # 背景2移动到背景1的正上方
    BackGround.background1.y += 1  # 背景1向下滚动
    BackGround.background2.y += 1  # 背景2向下滚动


pgzrun.go()  # begin gaming
