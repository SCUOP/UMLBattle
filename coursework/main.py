import pgzrun  # 导入游戏库
import random  # 导入随机库
from abc import abstractmethod, ABC
import math

WIDTH = 480  # 设置窗口的宽度
HEIGHT = 700  # 设置窗口的高度
TITLE = "Python Game"
# score = 0  # 游戏得分
# isLoose = False  # 游戏是否失败，初始不失败
# sounds.game_music.play(-1)  # 循环播放背景音乐


class Game:
    """game elements"""

    WIDTH = WIDTH  # set width of window
    HEIGHT = HEIGHT  # set height of window
    TITLE = TITLE  # set title of window
    deviation = 20  # map enlargement deviation value
    actors = []
    background = None
    hero = None
    start_button = None
    enemise = []
    lose = False
    start = False
    init = True

    @staticmethod
    def update():
        if Game.lose:
            Game.deal_lose()
            return
        Game.generate_actors()
        # while destroy all enemies, go to next level
        if Game.enemise == []:
            Game.next_level()

    @staticmethod
    def deal_lose():
        # deal when the player lose game
        # TODO:
        Game.init = True

    @staticmethod
    def generate_actors():
        Game.actors = []
        if Game.background != None:
            Game.actors += [Game.background]
        if Game.hero != None:
            Game.actors += [Game.hero]
        if Game.start_button != None:
            Game.actors += [Game.start_button]
        if Game.enemise != None and Game.enemise != []:
            Game.actors += Game.enemise

    # TODO: init game
    @staticmethod
    def init_game():
        Game.background = BackGround()
        Game.start_button = Start_Button()
        if Game.hero != None:
            Game.hero.forced_kill()
        Game.hero = Hero()
        for enemy in Game.enemise:
            enemy.forced_kill()
        Game.enemise.append(Basic_Enemy())
        Game.lose = False
        Game.start = False
        Game.init = False
        Game.update()

    def next_level():
        Game.start_button = Start_Button()
        Game.hero.forced_kill()
        Game.enemise.append(Basic_Enemy())
        Game.update()
        Game.start = False


# interface of all kinds of actors
class All_Actors(ABC):
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


class Actor_has_blood(All_Actors):
    @abstractmethod
    def be_attacked(self, decrease_value: int):
        pass

    @abstractmethod
    def check_blood(self):
        pass

    @abstractmethod
    def update_blood_pos(self):
        pass

    @abstractmethod
    def forced_kill(self):
        pass


class BackGround(All_Actors):
    def __init__(self) -> None:
        self.background1 = Actor("background")  # 导入背景1图片
        self.background1.x = self.background1.width / 2  # 背景1的x坐标
        self.background1.y = self.background1.height / 2  # 背景1的y坐标
        self.background2 = Actor("background")  # 导入背景2图片
        self.background2.x = self.background2.width / 2  # 背景2的x坐标
        self.background2.y = -self.background2.height / 2  # 背景2的y坐标

    def draw(self):
        self.background1.draw()  # 绘制游戏背景
        self.background2.draw()  # 绘制游戏背景

    def update(self):
        # 以下代码用于实现背景图片的循环滚动效果
        if self.background1.y > 852 / 2 + 852:
            self.background1.y = -852 / 2  # 背景1移动到背景2的正上方
        if self.background2.y > 852 / 2 + 852:
            self.background2.y = -852 / 2  # 背景2移动到背景1的正上方
        self.background1.y += 1  # 背景1向下滚动
        self.background2.y += 1  # 背景2向下滚动

    def get_actor(self):
        pass


class Start_Button(All_Actors):
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
        self.check_keyboard()

    def get_actor(self):
        return self.start_pic

    def check_keyboard(self):
        if keyboard.RETURN and self.start_pic == self.start_no:
            self.start_pic = self.start_yes
        elif not keyboard.RETURN and self.start_pic == self.start_yes:
            Game.start_button = None
            Game.start = True


# the blood of actor
class HP(All_Actors):
    def __init__(
        self, blood: int, attach_target: Actor_has_blood, size: int = 5
    ) -> None:
        """HP of each actor

        Args:
            blood (int): blood
            attach_target (Actor_has_blood): the actor of the target which is the owner of this HP instance
            size (int): the height of the blood strip. Defaults to 5.
        """
        self.full_blood = blood
        self.now_blood = self.full_blood
        self.left = attach_target.get_actor().left
        self.right = attach_target.get_actor().right
        self.top = attach_target.get_actor().bottom
        self.bottom = self.top + size
        self.height = size
        self.width = self.right - self.left
        self.health_width = self.width * (self.now_blood * 1.0 / self.full_blood)

    def draw(self):
        screen.draw.rect(Rect(self.left, self.top, self.width, self.height), "white")
        screen.draw.filled_rect(
            Rect(self.left + 1, self.top + 1, self.health_width - 2, self.height - 2),
            "red",
        )

    def update(self):
        self.health_width = self.width * (self.now_blood * 1.0 / self.full_blood)

    def update_pos(self, attach_target: Actor_has_blood):
        self.left = attach_target.get_actor().left
        self.right = attach_target.get_actor().right
        self.top = attach_target.get_actor().bottom
        self.bottom = self.top + self.height

    def get_actor(self):
        return None

    def decrease_blood(self, decrease_value: int):
        """decrease_blood

        Args:
            decrease_value (int): decrease value of blood
        """
        self.now_blood -= decrease_value

    def get_now_blood(self) -> int:
        return self.now_blood


# super class for all kinds of bullets
class Bullets(All_Actors):
    def check_collision(self, actor):
        pass


# The most common bullet
class Basic_Bullets(Bullets):
    def __init__(self, shoot_pos: tuple, target_pos: tuple = None) -> None:
        """Basic_Bullets init

        Args:
            shoot_pos (tuple): bullets position
            v (tuple, optional): bullets v. Defaults to (0, 4).
            target_pos (tuple, optional): target position. Defaults to None.
        """
        self.bullet = Actor("bullet")
        self.bullet.x = shoot_pos[0]
        self.bullet.y = shoot_pos[1]
        self.exsit = True  # determine whether the bullet is exsit
        self.abs_v = 4  # absolute speed
        if target_pos is not None:
            self.bullet.angle = self.bullet.angle_to(target_pos) + 90
        self.bullet.vx = self.abs_v * math.sin(math.radians(self.bullet.angle))
        self.bullet.vy = self.abs_v * math.cos(math.radians(self.bullet.angle))
        self.attack_power = 4

    def draw(self):
        self.bullet.draw()

    def update(self):
        self.update_pos()
        self.check_boundary()

    def update_pos(self):
        self.bullet.x += self.bullet.vx
        self.bullet.y += self.bullet.vy

    def get_actor(self):
        return self.bullet

    def check_collision(self, actor):
        """check cpllision

        Args:
            actor (All_Actors): the bullet target
        """
        if self.bullet.colliderect(actor.get_actor()):
            self.exsit = False
            actor.be_attacked(self.attack_power)

    def check_boundary(self):
        # out of boundary
        if (
            self.bullet.x < -Game.deviation
            or self.bullet.x > Game.WIDTH + Game.deviation
            or self.bullet.y > Game.HEIGHT + Game.deviation
            or self.bullet.y < -Game.deviation
        ):
            self.exsit = False

    def check_exsit(self):
        return self.exsit


# player
# TODO: 删除时记得取消schedule
class Hero(Actor_has_blood):
    def __init__(self) -> None:
        self.hero = Actor("hero")
        self.hero.x = Game.WIDTH / 2
        self.hero.y = Game.HEIGHT * 2 / 3
        self.hero.speed = 3
        self.bullets = []
        self.bullet_class = Basic_Bullets
        self.attack_speed = 0.8  # gap time for each bullet
        self.attacking = False  # if hero attacking, stop add attacking schedule
        self.nearest = None
        self.hp = HP(4, self)

    def draw(self):
        self.hp.draw()
        self.hero.draw()
        self.draw_bullets()

    def update(self):
        self.hp.update()
        self.update_bullets()
        # traverse enemies to get the nearest enemy and check the collision with bullets
        self.check_enemies()
        # avoid leaving the map
        self.check_boundary()
        # check the keyboard
        self.check_keyboard()
        # check blood
        self.check_blood()
        # update_blood
        self.update_blood_pos()

    def be_attacked(self, decrease_value: int):
        """while be attacked

        Args:
            decrease_value (int): decrease value of blood
        """
        self.hp.decrease_blood(decrease_value)

    def get_actor(self) -> Actor:
        return self.hero

    def draw_bullets(self):
        for bullet in self.bullets:
            bullet.draw()

    def update_bullets(self):
        for bullet in self.bullets:
            bullet.update()
            if not bullet.check_exsit():
                self.bullets.remove(bullet)

    def update_blood_pos(self):
        self.hp.update_pos(self)

    def check_blood(self):
        if self.hp.get_now_blood() <= 0:
            if self.attacking == True:
                clock.unschedule(self.attack)
                self.attacking = False
            Game.deal_lose()

    # traverse enemies to get the nearest enemy and check the collision with bullets
    def check_enemies(self):
        distance = 99999
        for actor in Game.enemise:
            if self.hero.distance_to(actor.get_actor()) < distance:
                distance = self.hero.distance_to(actor.get_actor())
                self.nearest = actor
            for bullet in self.bullets:
                bullet.check_collision(actor)

    # check keyboard event
    def check_keyboard(self):
        if self.press_key():
            self.attacking = False
            clock.unschedule(self.attack)
        else:
            # if not press key, excute other events
            # face the nearest enemy
            if self.nearest != None:
                self.hero.angle = self.hero.angle_to(self.nearest.get_actor()) - 90
            # shoot when the player dosen't move
            if not self.attacking:
                clock.schedule_interval(self.attack, self.attack_speed)
                self.attacking = True

    # shoot
    def attack(self):
        distance = self.hero.height / 2
        pos = (
            self.hero.x - distance * math.sin(math.radians(self.hero.angle)),
            self.hero.y - distance * math.cos(math.radians(self.hero.angle)),
        )
        self.bullets.append(self.bullet_class(pos, self.nearest.get_actor().pos))

    # avoid leaving the map
    def check_boundary(self):
        if self.hero.left < 0:
            self.hero.left = 0
        if self.hero.right > Game.WIDTH:
            self.hero.right = Game.WIDTH
        if self.hero.bottom > Game.HEIGHT:
            self.hero.bottom = Game.HEIGHT
        if self.hero.top < 0:
            self.hero.top = 0

    def press_key(self):
        if (
            not keyboard.UP
            and not keyboard.DOWN
            and not keyboard.LEFT
            and not keyboard.RIGHT
        ):
            return False
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
        return True

    def forced_kill(self):
        if self.attacking:
            clock.unschedule(self.attack)
            self.attacking = False
        self.bullets = []


class Enemy(Actor_has_blood):
    def attack(self):
        pass


# the most common enemy
# TODO:删除时记得取消schedule
class Basic_Enemy(Enemy):
    def __init__(self) -> None:
        self.enemy = Actor("enemy")
        self.enemy.x = Game.WIDTH / 2
        self.enemy.y = Game.HEIGHT / 4
        self.bullets = []
        self.bullet_class = Basic_Bullets
        self.attack_speed = 1.5  # gap time for each bullet
        self.attacking = False
        self.hp = HP(20, self)

    def draw(self):
        self.enemy.draw()
        self.draw_bullets()
        self.hp.draw()

    def update(self):
        self.update_self()
        self.update_bullets()
        # check blood
        self.check_blood()
        # update_blood
        self.update_blood_pos()
        self.hp.update()

    def be_attacked(self, decrease_value: int):
        self.hp.decrease_blood(decrease_value)

    def check_blood(self):
        # check the enemy is killed?
        if self.hp.get_now_blood() <= 0:
            if self.attacking:
                clock.unschedule(self.attack)
            Game.enemise.remove(self)

    def update_blood_pos(self):
        self.hp.update_pos(self)

    def draw_bullets(self):
        for bullet in self.bullets:
            bullet.draw()

    def update_self(self):
        self.adjust_angle()
        self.attack_schedule()

    def adjust_angle(self):
        # enemy towards the player
        if Game.hero != None:
            self.enemy.angle = self.enemy.angle_to(Game.hero.get_actor()) + 90

    def attack_schedule(self):
        if not self.attacking:
            clock.schedule_interval(self.attack, self.attack_speed)
            self.attacking = True

    def update_bullets(self):
        for bullet in self.bullets:
            bullet.update()
            bullet.check_collision(Game.hero)
            if not bullet.check_exsit():
                self.bullets.remove(bullet)

    def attack(self):  # shoot
        distance = self.enemy.height / 2
        pos = (
            self.enemy.x + distance * math.sin(math.radians(self.enemy.angle)),
            self.enemy.y + distance * math.cos(math.radians(self.enemy.angle)),
        )
        self.bullets.append(self.bullet_class(pos, Game.hero.get_actor().pos))

    def get_actor(self):
        return self.enemy

    # only Game progress can force kill
    def forced_kill(self):
        if self.attacking:
            clock.unschedule(self.attack)
            self.attacking = False
        Game.enemise.remove(self)


def draw():  # 绘制模块，每帧重复执行
    for actor in Game.actors:
        actor.draw()


def update():  # 更新模块，每帧重复操作
    if Game.init:
        Game.init_game()
    Game.update()
    if not Game.lose:
        if not Game.start:
            Game.start_button.update()
        else:
            for actor in Game.actors:
                actor.update()


pgzrun.go()  # begin gaming
