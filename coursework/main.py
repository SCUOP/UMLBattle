import pgzrun  # 导入游戏库
import random  # 导入随机库
from abc import abstractmethod, ABC
import math
import copy

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
    barriers = []
    spider_webs = []
    thornses = []
    lose = False
    start = False
    init = True
    level = 1
    choose_buff = False
    buff = []
    buff_cursor = 1
    buff_choice = []
    buff_pressdown = False
    buff_take_effect = False

    @staticmethod
    def update():
        if Game.lose:
            Game.deal_lose()
            return
        Game.generate_actors()
        # while destroy all enemies, go to next level
        if Game.enemise == [] and Game.start:
            Game.level += 1
            Game.next_level()
        # choose a buff
        if Game.choose_buff:
            Game.buff_choice[0](1, Game.buff_cursor == 1)
            Game.buff_choice[1](2, Game.buff_cursor == 2)
            Game.buff_choice[2](3, Game.buff_cursor == 3)
            Game.move_buff_cursor()

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
        if Game.barriers != None and Game.barriers != []:
            Game.actors += Game.barriers
        if Game.spider_webs != None and Game.spider_webs != []:
            Game.actors += Game.spider_webs
        if Game.thornses != None and Game.thornses != []:
            Game.actors += Game.thornses
        if Game.hero != None:
            Game.actors += [Game.hero]
        if Game.enemise != None and Game.enemise != []:
            Game.actors += Game.enemise
        if Game.start_button != None:
            Game.actors += [Game.start_button]

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
        Game.barriers = []
        Game.spider_webs = []
        Game.thornses = []
        Game.lose = False
        Game.start = False
        Game.level = 1
        Game.init = False
        Game.buff = [
            Buff.increase_damage,
            Buff.slow_down_enemies,
            Buff.rebound,
            Buff.add_front_bullet,
            Buff.add_attack_speed,
            Buff.recover_hp,
            Buff.add_left_top_right_bullet,
            Buff.add_left_right_bullet,
            Buff.pass_bullet,
            Buff.speed_up_bullet,
        ]
        Game.next_level()
        # Game.update()

    @staticmethod
    def next_level():
        Game.start_button = Start_Button()
        Game.hero.forced_kill()
        Game.enemise = []
        Game.barriers = []
        Game.spider_webs = []
        Game.thornses = []
        # TODO: 修改
        if Game.level > 5:
            Game.level = 1
        Game.level_generator[Game.level - 1]()
        Game.start = False
        if Game.level != 1:
            Game.choose_buff = True
            Game.buff_choice = random.sample(Game.buff, 3)

    @staticmethod
    def move_buff_cursor():
        if not Game.buff_pressdown and keyboard.UP:
            Game.buff_cursor -= 1
            Game.buff_pressdown = True
        elif not Game.buff_pressdown and keyboard.DOWN:
            Game.buff_cursor += 1
            Game.buff_pressdown = True
        elif not keyboard.UP and not keyboard.DOWN and not keyboard.RETURN:
            Game.buff_pressdown = False
        if not keyboard.RETURN and Game.buff_take_effect:
            Game.buff_take_effect = False
            Game.buff_pressdown = False
            Game.choose_buff = False
        if Game.buff_cursor < 1:
            Game.buff_cursor = 3
        if Game.buff_cursor > 3:
            Game.buff_cursor = 1

    @staticmethod
    def level1():
        Game.enemise += [
            Basic_Enemy(
                x=random.uniform(Game.deviation, Game.WIDTH * 1.0 / 3),
                y=random.uniform(Game.deviation, Game.HEIGHT * 1.0 / 4),
            ),
            Basic_Enemy(
                x=random.uniform(
                    Game.deviation + Game.WIDTH * 2.0 / 3,
                    Game.WIDTH - Game.deviation,
                ),
                y=random.uniform(Game.deviation, Game.HEIGHT * 1.0 / 4),
            ),
            Move_Enemy(
                x=random.uniform(
                    Game.deviation + Game.WIDTH * 1.0 / 3,
                    Game.WIDTH * 2.0 / 3 - Game.deviation,
                ),
                y=random.uniform(Game.deviation, Game.HEIGHT * 1.0 / 4),
            ),
        ]
        # Game.barriers.append(Barrier())
        Game.barriers += [
            Barrier(
                x=random.uniform(0, Game.WIDTH * 1.0 / 3),
                y=random.uniform(Game.HEIGHT * 1.0 / 3, Game.HEIGHT * 2.0 / 3),
            ),
            Barrier(
                x=random.uniform(Game.WIDTH * 1.0 / 3, Game.WIDTH * 2.0 / 3),
                y=random.uniform(Game.HEIGHT * 1.0 / 3, Game.HEIGHT * 2.0 / 3),
            ),
            Barrier(
                x=random.uniform(Game.WIDTH * 2.0 / 3, Game.WIDTH),
                y=random.uniform(Game.HEIGHT * 1.0 / 3, Game.HEIGHT * 2.0 / 3),
            ),
        ]
        Game.spider_webs += [
            Spider_Web(
                x=random.uniform(0, Game.WIDTH * 1.0 / 3),
                y=random.uniform(Game.HEIGHT * 3.0 / 5, Game.HEIGHT * 4.0 / 5),
            ),
            Spider_Web(
                x=random.uniform(Game.WIDTH * 2.0 / 3, Game.WIDTH),
                y=random.uniform(Game.HEIGHT * 3.0 / 5, Game.HEIGHT * 4.0 / 5),
            ),
        ]
        Game.thornses += [
            Thorns(
                x=random.uniform(0, Game.WIDTH * 1.0 / 3),
                y=random.uniform(Game.HEIGHT * 3.0 / 5, Game.HEIGHT * 4.0 / 5),
            ),
            Thorns(
                x=random.uniform(Game.WIDTH * 2.0 / 3, Game.WIDTH),
                y=random.uniform(Game.HEIGHT * 3.0 / 5, Game.HEIGHT * 4.0 / 5),
            ),
        ]

    @staticmethod
    def level2():
        Game.level1()

    @staticmethod
    def level3():
        Game.level1()

    @staticmethod
    def level4():
        Game.level1()

    @staticmethod
    def level5():
        Game.level1()

    level_generator = [
        level1,
        level2,
        level3,
        level4,
        level5,
    ]  # generate the different level

    @staticmethod
    def check_actor_collide(actor1: Actor, actor2: Actor):
        if (
            actor1.x + actor1.width / 2 >= actor2.x - actor2.width / 2
            and actor1.x - actor1.width / 2 <= actor2.x + actor2.width / 2
            and actor1.y + actor1.height / 2 >= actor2.y - actor2.height / 2
            and actor1.y - actor1.height / 2 <= actor2.y + actor2.height / 2
        ):
            return True
        return False


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
    def get_actor(self) -> Actor:
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

    @abstractmethod
    def recover(self, recover_blood: int = 10):
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
        # self.background2.draw()  # 绘制游戏背景

    def update(self):
        pass
        # # 以下代码用于实现背景图片的循环滚动效果
        # if self.background1.y > 852 / 2 + 852:
        #     self.background1.y = -852 / 2  # 背景1移动到背景2的正上方
        # if self.background2.y > 852 / 2 + 852:
        #     self.background2.y = -852 / 2  # 背景2移动到背景1的正上方
        # self.background1.y += 1  # 背景1向下滚动
        # self.background2.y += 1  # 背景2向下滚动

    def get_actor(self):
        return self.background1


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
        if Game.choose_buff:
            return
        self.start_pic.draw()

    def update(self):
        if Game.choose_buff or Game.buff_pressdown:
            return
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
        screen.draw.rect(Rect(self.left, self.top, self.width, self.height), "black")
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
    @abstractmethod
    def check_collision(self, actor):
        pass

    @abstractmethod
    def check_boundary(self):
        pass

    @abstractmethod
    def check_exsit(self) -> bool:
        pass

    @abstractmethod
    def get_copy(self):
        pass


# The most common bullet
class Basic_Bullets(Bullets):
    def __init__(
        self,
        shoot_pos: tuple = (Game.WIDTH / 2, Game.HEIGHT / 2),
        target_pos: tuple = (Game.WIDTH / 2, Game.HEIGHT / 2),
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
        self.bullet.abs_v = 4  # absolute speed
        self.bullet.angle = self.bullet.angle_to(target_pos) + 90
        self.bullet.vx = self.bullet.abs_v * math.sin(math.radians(self.bullet.angle))
        self.bullet.vy = self.bullet.abs_v * math.cos(math.radians(self.bullet.angle))
        self.bullet.exsit = True  # determine whether the bullet is exsit
        self.bullet.attack_power = 4
        self.bullet.enemies = copy.copy(
            Game.enemise
        )  # rebound and pass_bullet need this attr to avoid repeat attack

    def draw(self):
        self.bullet.draw()

    def update(self):
        self.update_pos()
        self.check_boundary()

    def update_pos(self):
        self.bullet.x += self.bullet.vx
        self.bullet.y += self.bullet.vy

    def get_actor(self) -> Actor:
        return self.bullet

    def check_collision(self, actor: Actor_has_blood):
        """check cpllision

        Args:
            actor (All_Actors): the bullet target
        """
        if Game.check_actor_collide(self.get_actor(), actor.get_actor()):
            self.bullet.exsit = False
            actor.be_attacked(self.bullet.attack_power)

    # out of boundary or hit the wall
    def check_boundary(self):
        # out of boundary
        if (
            self.bullet.x < -Game.deviation
            or self.bullet.x > Game.WIDTH + Game.deviation
            or self.bullet.y > Game.HEIGHT + Game.deviation
            or self.bullet.y < -Game.deviation
        ):
            self.bullet.exsit = False
        for barrier in Game.barriers:
            if Game.check_actor_collide(self.get_actor(), barrier.get_actor()):
                self.bullet.exsit = False

    def check_exsit(self) -> bool:
        return self.bullet.exsit

    def get_copy(self) -> Bullets:
        bullet = Basic_Bullets()
        bullet.bullet.pos = self.bullet.pos
        bullet.bullet.abs_v = self.bullet.abs_v
        bullet.bullet.angle = self.bullet.angle
        bullet.bullet.vx = self.bullet.vx
        bullet.bullet.vy = self.bullet.vy
        bullet.bullet.exsit = self.bullet.exsit
        bullet.bullet.attack_power = self.bullet.attack_power
        return bullet


# decorator of bullets
class Bullets_Decorator(Bullets):
    def __init__(self, bullet: Bullets) -> None:
        """the abstract decorator of bullets

        Args:
            bullet (Bullets): which bullet needs to be decorator
        """
        self.bullet = bullet

    # draw
    def draw(self):
        self.bullet.draw()

    # update
    def update(self):
        self.bullet.update()

    def get_actor(self) -> Actor:
        return self.bullet.get_actor()

    def check_collision(self, actor: Actor_has_blood):
        self.bullet.check_collision(actor)

    def check_boundary(self):
        self.bullet.check_boundary()

    def check_exsit(self) -> bool:
        return self.bullet.check_exsit()

    def get_copy(self) -> Bullets:
        return Bullets_Decorator(self.bullet.get_copy())


class Increase_Damage_Bullets_Decorator(Bullets_Decorator):
    def __init__(self, bullet: Bullets) -> None:
        super().__init__(bullet)
        self.get_actor().attack_power += 2

    def get_copy(self) -> Bullets:
        return Increase_Damage_Bullets_Decorator(self.bullet.get_copy())


class Slow_Down_Enemies_Bullets_Decorator(Bullets_Decorator):
    def check_collision(self, actor: Actor_has_blood):
        super().check_collision(actor)
        if hasattr(actor, "speed") and Game.check_actor_collide(
            self.get_actor(), actor.get_actor()
        ):
            actor.slow_down()

    def get_copy(self) -> Bullets:
        return Slow_Down_Enemies_Bullets_Decorator(self.bullet.get_copy())


class Rebound_Bullets_Decorator(Bullets_Decorator):
    def __init__(self, bullet: Bullets) -> None:
        super().__init__(bullet)
        # number of bounces (3)
        self.rebound_times = 3

    def update(self):
        self.update_pos()
        self.check_boundary()

    def check_boundary(self):
        super().check_boundary()
        # left and right boundary
        if (
            self.get_actor().x <= self.get_actor().width / 2
            or self.get_actor().x >= Game.WIDTH - self.get_actor().width / 2
        ):
            self.get_actor().vx = -self.get_actor().vx
            self.rebound_times -= 1
            self.get_actor().enemies = copy.copy(Game.enemise)
        # top and bottom boundary
        elif (
            self.get_actor().y <= self.get_actor().height / 2
            or self.get_actor().y >= Game.HEIGHT - self.get_actor().height / 2
        ):
            self.get_actor().vy = -self.get_actor().vy
            self.rebound_times -= 1
            self.get_actor().enemies = copy.copy(Game.enemise)
        # hit barrier
        for barrier in Game.barriers:
            if Game.check_actor_collide(self.get_actor(), barrier.get_actor()):
                self.get_actor().enemies = copy.copy(Game.enemise)
                self.rebound_times -= 1
                if (
                    self.get_actor().y >= barrier.get_actor().top
                    and self.get_actor().y <= barrier.get_actor().bottom
                ):
                    self.get_actor().vx = -self.get_actor().vx
                elif (
                    self.get_actor().x >= barrier.get_actor().left
                    and self.get_actor().x <= barrier.get_actor().right
                ):
                    self.get_actor().vy = -self.get_actor().vy
                else:
                    self.get_actor().vx = -self.get_actor().vx
                    self.get_actor().vy = -self.get_actor().vy
        if self.rebound_times < 0:
            self.get_actor().exsit = False
        else:
            self.get_actor().exsit = True

    def update_pos(self):
        self.get_actor().x += self.get_actor().vx
        self.get_actor().y += self.get_actor().vy

    def get_copy(self) -> Bullets:
        return Rebound_Bullets_Decorator(self.bullet.get_copy())


class Pass_Bullet_Bullets_Decorator(Bullets_Decorator):
    def check_collision(self, actor: Actor_has_blood):
        super().check_collision(actor)
        if Game.check_actor_collide(self.get_actor(), actor.get_actor()):
            self.get_actor().exsit = True
            actor.recover(self.get_actor().attack_power)
            if actor in self.get_actor().enemies:
                self.get_actor().enemies.remove(actor)
                actor.be_attacked(self.get_actor().attack_power)

    def get_copy(self) -> Bullets:
        return Pass_Bullet_Bullets_Decorator(self.bullet.get_copy())


class Speed_Up_Bullet_Bullets_Decorator(Bullets_Decorator):
    def __init__(self, bullet: Bullets) -> None:
        super().__init__(bullet)
        self.get_actor().abs_v += 2
        self.get_actor().vx = self.get_actor().abs_v * math.sin(
            math.radians(self.get_actor().angle)
        )
        self.get_actor().vy = self.get_actor().abs_v * math.cos(
            math.radians(self.get_actor().angle)
        )

    def get_copy(self) -> Bullets:
        return Speed_Up_Bullet_Bullets_Decorator(self.bullet.get_copy())


class Barrier(All_Actors):
    def __init__(
        self,
        actor_pic: str = "barrier",
        x: float = Game.WIDTH / 2,
        y: float = Game.HEIGHT / 2,
    ) -> None:
        """init basic enemy

        Args:
            actor_pic (str, optional): Defaults to Actor("barrier").
            x (int, optional): Defaults to Game.WIDTH/2.
            y (int, optional): Defaults to Game.HEIGHT/4.
        """
        self.barrier = Actor(actor_pic)
        self.barrier.x = x
        self.barrier.y = y

    def draw(self):
        self.barrier.draw()

    def update(self):
        pass

    def get_actor(self):
        return self.barrier


class Spider_Web(All_Actors):
    def __init__(
        self,
        actor_pic: str = "spider_web",
        x: float = Game.WIDTH / 2,
        y: float = Game.HEIGHT / 2,
    ) -> None:
        """init basic enemy

        Args:
            actor_pic (str, optional): Defaults to Actor("barrier").
            x (int, optional): Defaults to Game.WIDTH/2.
            y (int, optional): Defaults to Game.HEIGHT/4.
        """
        self.spider_web = Actor(actor_pic)
        self.spider_web.x = x
        self.spider_web.y = y

    def draw(self):
        self.spider_web.draw()

    def update(self):
        if Game.check_actor_collide(self.spider_web, Game.hero.get_actor()):
            Game.hero.slow_down()

    def get_actor(self):
        return self.spider_web


class Thorns(All_Actors):
    def __init__(
        self,
        actor_pic: str = "thorns",
        x: float = Game.WIDTH / 2,
        y: float = Game.HEIGHT / 2,
    ) -> None:
        """init basic enemy

        Args:
            actor_pic (str, optional): Defaults to Actor("barrier").
            x (int, optional): Defaults to Game.WIDTH/2.
            y (int, optional): Defaults to Game.HEIGHT/4.
        """
        self.thorns = Actor(actor_pic)
        self.thorns.x = x
        self.thorns.y = y
        self.attack_power = 4
        self.attacking = False

    def draw(self):
        self.thorns.draw()

    def update(self):
        if not self.attacking and Game.check_actor_collide(
            self.thorns, Game.hero.get_actor()
        ):
            Game.hero.be_attacked(self.attack_power)
            self.attacking = True
            clock.schedule_unique(self.unlock_attack, 1)

    def get_actor(self):
        return self.thorns

    def unlock_attack(self):
        self.attacking = False


# player
# TODO: 删除时记得取消schedule
class Hero(Actor_has_blood):
    def __init__(self) -> None:
        self.hero = Actor("hero")
        self.hero.x = Game.WIDTH / 2
        self.hero.y = Game.HEIGHT * 4 / 5
        self.hero.speed = 2.5  # move speed
        self.previous_pos = self.hero.pos
        self.bullets = []
        self.bullet_proto = Pass_Bullet_Bullets_Decorator(
            Rebound_Bullets_Decorator(Basic_Bullets())
        )
        self.bullet_class = Bullets_Decorator
        self.attack_speed = 1.2  # gap time for each bullet
        self.attacking = False  # if hero attacking, stop add attacking schedule
        self.nearest = None
        self.add_front_bullet = False
        self.add_left_top_right_bullet = False
        self.add_left_right_bullet = False
        self.hp = HP(50, self)

    def draw(self):
        self.hp.draw()
        self.hero.draw()
        self.draw_bullets()

    def update(self):
        self.hp.update()
        self.update_bullets()
        # traverse enemies to get the nearest enemy and check the collision with bullets
        self.check_enemies()
        # check the keyboard
        self.update_hero_pos()
        # avoid leaving the map or hit the wall
        self.check_boundary()
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
            if not bullet.check_exsit():
                self.bullets.remove(bullet)
            bullet.update()

    def update_blood_pos(self):
        self.hp.update_pos(self)

    def check_blood(self):
        if self.hp.get_now_blood() <= 0:
            if self.attacking == True:
                clock.unschedule(self.attack)
                self.attacking = False
            Game.lose = True

    # traverse enemies to get the nearest enemy and check the collision with bullets
    def check_enemies(self):
        distance = 99999
        for actor in Game.enemise:
            if self.hero.distance_to(actor.get_actor()) < distance:
                distance = self.hero.distance_to(actor.get_actor())
                self.nearest = actor
            for bullet in self.bullets:
                bullet.check_collision(actor)

    # update hero position
    def update_hero_pos(self):
        self.previous_pos = self.hero.pos
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
        bullet = self.bullet_proto.get_copy()
        bullet.get_actor().pos = pos
        bullet.get_actor().angle = (
            bullet.get_actor().angle_to(self.nearest.get_actor().pos) + 90
        )
        bullet.get_actor().vx = bullet.get_actor().abs_v * math.sin(
            math.radians(bullet.get_actor().angle)
        )
        bullet.get_actor().vy = bullet.get_actor().abs_v * math.cos(
            math.radians(bullet.get_actor().angle)
        )
        self.bullets.append(self.bullet_class(bullet))
        if self.add_front_bullet:
            left_bullet = bullet.get_copy()
            left_bullet.get_actor().x += left_bullet.get_actor().width * math.cos(
                math.radians(left_bullet.get_actor().angle)
            )
            left_bullet.get_actor().y -= left_bullet.get_actor().width * math.sin(
                math.radians(left_bullet.get_actor().angle)
            )
            self.bullets.append(self.bullet_class(left_bullet))
            right_bullet = bullet.get_copy()
            right_bullet.get_actor().x -= left_bullet.get_actor().width * math.cos(
                math.radians(left_bullet.get_actor().angle)
            )
            right_bullet.get_actor().y += left_bullet.get_actor().width * math.sin(
                math.radians(left_bullet.get_actor().angle)
            )
            self.bullets.append(self.bullet_class(right_bullet))
        if self.add_left_top_right_bullet:
            left_bullet = bullet.get_copy()
            pos = (
                self.hero.x - distance * math.sin(math.radians(self.hero.angle + 45)),
                self.hero.y - distance * math.cos(math.radians(self.hero.angle + 45)),
            )
            left_bullet.get_actor().pos = pos
            left_bullet.get_actor().angle = (
                left_bullet.get_actor().angle_to(self.nearest.get_actor().pos) + 135
            )
            left_bullet.get_actor().vx = left_bullet.get_actor().abs_v * math.sin(
                math.radians(left_bullet.get_actor().angle)
            )
            left_bullet.get_actor().vy = left_bullet.get_actor().abs_v * math.cos(
                math.radians(left_bullet.get_actor().angle)
            )
            self.bullets.append(self.bullet_class(left_bullet))
            right_bullet = bullet.get_copy()
            pos = (
                self.hero.x - distance * math.sin(math.radians(self.hero.angle - 45)),
                self.hero.y - distance * math.cos(math.radians(self.hero.angle - 45)),
            )
            right_bullet.get_actor().pos = pos
            right_bullet.get_actor().angle = (
                right_bullet.get_actor().angle_to(self.nearest.get_actor().pos) + 45
            )
            right_bullet.get_actor().vx = right_bullet.get_actor().abs_v * math.sin(
                math.radians(right_bullet.get_actor().angle)
            )
            right_bullet.get_actor().vy = right_bullet.get_actor().abs_v * math.cos(
                math.radians(right_bullet.get_actor().angle)
            )
            self.bullets.append(self.bullet_class(right_bullet))
        if self.add_left_right_bullet:
            left_bullet = bullet.get_copy()
            pos = (
                self.hero.x - distance * math.sin(math.radians(self.hero.angle + 90)),
                self.hero.y - distance * math.cos(math.radians(self.hero.angle + 90)),
            )
            left_bullet.get_actor().pos = pos
            left_bullet.get_actor().angle = (
                left_bullet.get_actor().angle_to(self.nearest.get_actor().pos) + 180
            )
            left_bullet.get_actor().vx = left_bullet.get_actor().abs_v * math.sin(
                math.radians(left_bullet.get_actor().angle)
            )
            left_bullet.get_actor().vy = left_bullet.get_actor().abs_v * math.cos(
                math.radians(left_bullet.get_actor().angle)
            )
            self.bullets.append(self.bullet_class(left_bullet))
            right_bullet = bullet.get_copy()
            pos = (
                self.hero.x - distance * math.sin(math.radians(self.hero.angle - 90)),
                self.hero.y - distance * math.cos(math.radians(self.hero.angle - 90)),
            )
            right_bullet.get_actor().pos = pos
            right_bullet.get_actor().angle = right_bullet.get_actor().angle_to(
                self.nearest.get_actor().pos
            )
            right_bullet.get_actor().vx = right_bullet.get_actor().abs_v * math.sin(
                math.radians(right_bullet.get_actor().angle)
            )
            right_bullet.get_actor().vy = right_bullet.get_actor().abs_v * math.cos(
                math.radians(right_bullet.get_actor().angle)
            )
            self.bullets.append(self.bullet_class(right_bullet))

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
        for barrier in Game.barriers:
            if Game.check_actor_collide(self.get_actor(), barrier.get_actor()):
                self.hero.pos = self.previous_pos

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
        self.hero.x = Game.WIDTH / 2
        self.hero.y = Game.HEIGHT * 4 / 5
        self.hero.angle = 0
        self.update_blood_pos()

    # upgrade the bullet
    def upgrade_bullet(self, upgrade_bullet_class: Bullets):
        self.bullet_proto = self.bullet_class(self.bullet_proto)
        self.bullet_class = upgrade_bullet_class

    def increase_attack_speed(self) -> bool:
        self.attack_speed -= 0.4
        if self.attack_speed == 0.4:
            return False
        return True

    def recover(self, recover_blood: int = 10):
        self.hp.now_blood += recover_blood
        if self.hp.now_blood > self.hp.full_blood:
            self.hp.now_blood = self.hp.full_blood

    def slow_down(self):
        self.hero.speed = 1
        clock.schedule_unique(self.recover_speed, 0.05)

    def recover_speed(self):
        self.hero.speed = 2.5


class Enemy(Actor_has_blood):
    def attack(self):
        pass


# the most common enemy
# TODO:删除时记得取消schedule
class Basic_Enemy(Enemy):
    def __init__(
        self,
        actor_pic: str = "enemy",
        x: int = Game.WIDTH / 2,
        y: int = Game.HEIGHT / 4,
    ) -> None:
        """init basic enemy

        Args:
            actor_pic (str, optional): Defaults to Actor("enemy").
            x (int, optional): Defaults to Game.WIDTH/2.
            y (int, optional): Defaults to Game.HEIGHT/4.
        """
        self.enemy = Actor(actor_pic)
        self.enemy.x = x
        self.enemy.y = y
        self.bullets = []
        self.bullet_class = Basic_Bullets
        self.attack_speed = 1.5  # gap time for each bullet
        self.attacking = False
        self.hp = HP(16, self)

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

    def recover(self, recover_blood: int = 10):
        self.hp.now_blood += recover_blood


class Sniper_Enemy(Basic_Enemy):
    class Sniper_Bullet(Bullets_Decorator):
        def __init__(
            self,
            shoot_pos: tuple = (Game.WIDTH / 2, Game.HEIGHT / 2),
            target_pos: tuple = (Game.WIDTH / 2, Game.HEIGHT / 2),
        ) -> None:
            self.bullet = Speed_Up_Bullet_Bullets_Decorator(
                Speed_Up_Bullet_Bullets_Decorator(
                    Increase_Damage_Bullets_Decorator(
                        Increase_Damage_Bullets_Decorator(
                            Increase_Damage_Bullets_Decorator(
                                Basic_Bullets(
                                    shoot_pos=shoot_pos, target_pos=target_pos
                                )
                            )
                        )
                    )
                )
            )

    def __init__(
        self,
        actor_pic: str = "enemy",
        x: int = Game.WIDTH / 2,
        y: int = Game.HEIGHT / 4,
    ) -> None:
        super().__init__(actor_pic, x, y)
        self.attack_speed = 2.5
        self.bullet_class = Sniper_Enemy.Sniper_Bullet


class Machine_Gun_Enemy(Basic_Enemy):
    def fire_one_bullet(self):
        distance = self.enemy.height / 2
        pos = (
            self.enemy.x + distance * math.sin(math.radians(self.enemy.angle)),
            self.enemy.y + distance * math.cos(math.radians(self.enemy.angle)),
        )
        self.bullets.append(self.bullet_class(pos, Game.hero.get_actor().pos))

    def attack(self):  # shoot
        for i in range(6):
            clock.schedule(self.fire_one_bullet, 0.1 * (i + 1))


class Shotgun_Machine_Gun_Enemy(Machine_Gun_Enemy):
    def fire_one_bullet(self):
        distance = self.enemy.height / 2
        pos = (
            self.enemy.x + distance * math.sin(math.radians(self.enemy.angle)),
            self.enemy.y + distance * math.cos(math.radians(self.enemy.angle)),
        )
        self.bullets.append(self.bullet_class(pos, Game.hero.get_actor().pos))
        pos = (
            self.enemy.x - distance * math.sin(math.radians(self.enemy.angle + 45)),
            self.enemy.y - distance * math.cos(math.radians(self.enemy.angle + 45)),
        )
        self.bullets.append(self.bullet_class(pos, Game.hero.get_actor().pos))
        pos = (
            self.enemy.x - distance * math.sin(math.radians(self.enemy.angle - 45)),
            self.enemy.y - distance * math.cos(math.radians(self.enemy.angle - 45)),
        )
        self.bullets.append(self.bullet_class(pos, Game.hero.get_actor().pos))


class Move_Enemy(Enemy):
    def __init__(
        self,
        actor_pic: str = "enemy",
        x: int = Game.WIDTH / 2,
        y: int = Game.HEIGHT / 4,
    ) -> None:
        """init basic enemy

        Args:
            actor_pic (str, optional): Defaults to Actor("enemy").
            x (int, optional): Defaults to Game.WIDTH/2.
            y (int, optional): Defaults to Game.HEIGHT/4.
        """
        self.enemy = Actor(actor_pic)
        self.enemy.x = x
        self.enemy.y = y
        self.speed = 1  # speed
        self.enemy.angle = self.enemy.angle_to(Game.hero.get_actor()) + 90
        self.vx = self.speed * math.sin(math.radians(self.enemy.angle))
        self.vy = self.speed * math.cos(math.radians(self.enemy.angle))
        self.attacking = False
        self.hp = HP(24, self)
        self.attack_power = 10

    # draw
    def draw(self):
        self.enemy.draw()
        self.hp.draw()

    # update
    def update(self):
        self.update_self()
        # check blood
        self.check_blood()
        # update_blood
        self.update_blood_pos()
        self.hp.update()

    def get_actor(self) -> Actor:
        return self.enemy

    def be_attacked(self, decrease_value: int):
        self.hp.decrease_blood(decrease_value)

    def update_self(self):
        self.adjust_angle()
        if not self.attacking:
            self.move()
            self.attack()

    def adjust_angle(self):
        # enemy towards the player
        if Game.hero != None:
            self.enemy.angle = self.enemy.angle_to(Game.hero.get_actor()) + 90
            self.vx = self.speed * math.sin(math.radians(self.enemy.angle))
            self.vy = self.speed * math.cos(math.radians(self.enemy.angle))

    def move(self):
        self.enemy.x += self.vx
        self.enemy.y += self.vy

    def check_blood(self):
        # check the enemy is killed?
        if self.hp.get_now_blood() <= 0:
            Game.enemise.remove(self)

    def update_blood_pos(self):
        self.hp.update_pos(self)

    def forced_kill(self):
        Game.enemise.remove(self)

    def attack(self):
        if Game.check_actor_collide(self.enemy, Game.hero.get_actor()):
            Game.hero.be_attacked(self.attack_power)
            self.attacking = True
            clock.schedule(self.unlock_attack, 1.0)

    def unlock_attack(self):
        self.attacking = False

    def slow_down(self):
        self.speed = 0.5
        clock.schedule_unique(self.recover_speed, 2.0)

    def recover_speed(self):
        self.speed = 1

    def recover(self, recover_blood: int = 10):
        self.hp.now_blood += recover_blood


# TODO: Boss
class Boss:
    pass


class Buff:
    """order: int between 1 - 3 to define the order of each buff tag
    be_selected: bool check the cursor location
    """

    buff_pos = [
        (Game.WIDTH * 1.0 / 2, Game.HEIGHT * 1.0 / 4),
        (Game.WIDTH * 1.0 / 2, Game.HEIGHT * 2.0 / 4),
        (Game.WIDTH * 1.0 / 2, Game.HEIGHT * 3.0 / 4),
    ]

    increase_damage_tuple = (
        Actor("increase_damage"),
        Actor("increase_damage_selected"),
    )

    @staticmethod
    def increase_damage(order: int, be_selected: bool):
        Buff.increase_damage_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(Buff.increase_damage_tuple[be_selected])  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            Game.hero.upgrade_bullet(Increase_Damage_Bullets_Decorator)
            Game.buff_take_effect = True

    slow_down_enemies_tuple = (
        Actor("slow_down_enemies"),
        Actor("slow_down_enemies_selected"),
    )

    @staticmethod
    def slow_down_enemies(order: int, be_selected: bool):
        Buff.slow_down_enemies_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(
            Buff.slow_down_enemies_tuple[be_selected]
        )  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            Game.hero.upgrade_bullet(Slow_Down_Enemies_Bullets_Decorator)
            Game.buff.remove(Buff.slow_down_enemies)
            Game.buff_take_effect = True

    rebound_tuple = (
        Actor("rebound"),
        Actor("rebound_selected"),
    )

    @staticmethod
    def rebound(order: int, be_selected: bool):
        Buff.rebound_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(Buff.rebound_tuple[be_selected])  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            Game.hero.upgrade_bullet(Rebound_Bullets_Decorator)
            Game.buff.remove(Buff.rebound)
            Game.buff_take_effect = True

    add_front_bullet_tuple = (
        Actor("add_front_bullet"),
        Actor("add_front_bullet_selected"),
    )

    @staticmethod
    def add_front_bullet(order: int, be_selected: bool):
        Buff.add_front_bullet_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(Buff.add_front_bullet_tuple[be_selected])  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            Game.hero.add_front_bullet = True
            Game.buff.remove(Buff.add_front_bullet)
            Game.buff_take_effect = True

    add_attack_speed_tuple = (
        Actor("add_attack_speed"),
        Actor("add_attack_speed_selected"),
    )

    @staticmethod
    def add_attack_speed(order: int, be_selected: bool):
        Buff.add_attack_speed_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(Buff.add_attack_speed_tuple[be_selected])  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            if not Game.hero.increase_attack_speed():
                Game.buff.remove(Buff.add_attack_speed)
            Game.buff_take_effect = True

    recover_hp_tuple = (
        Actor("recover_hp"),
        Actor("recover_hp_selected"),
    )

    @staticmethod
    def recover_hp(order: int, be_selected: bool):
        Buff.recover_hp_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(Buff.recover_hp_tuple[be_selected])  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            Game.hero.recover()
            Game.buff_take_effect = True

    add_left_top_right_bullet_tuple = (
        Actor("add_left_top_right_bullet"),
        Actor("add_left_top_right_bullet_selected"),
    )

    @staticmethod
    def add_left_top_right_bullet(order: int, be_selected: bool):
        Buff.add_left_top_right_bullet_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(
            Buff.add_left_top_right_bullet_tuple[be_selected]
        )  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            Game.hero.add_left_top_right_bullet = True
            Game.buff.remove(Buff.add_left_top_right_bullet)
            Game.buff_take_effect = True

    add_left_right_bullet_tuple = (
        Actor("add_left_right_bullet"),
        Actor("add_left_right_bullet_selected"),
    )

    @staticmethod
    def add_left_right_bullet(order: int, be_selected: bool):
        Buff.add_left_right_bullet_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(
            Buff.add_left_right_bullet_tuple[be_selected]
        )  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            Game.hero.add_left_right_bullet = True
            Game.buff.remove(Buff.add_left_right_bullet)
            Game.buff_take_effect = True

    pass_bullet_tuple = (
        Actor("pass_bullet"),
        Actor("pass_bullet_selected"),
    )

    @staticmethod
    def pass_bullet(order: int, be_selected: bool):
        Buff.pass_bullet_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(Buff.pass_bullet_tuple[be_selected])  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            Game.hero.upgrade_bullet(Pass_Bullet_Bullets_Decorator)
            Game.buff.remove(Buff.pass_bullet)
            Game.buff_take_effect = True

    speed_up_bullet_tuple = (
        Actor("speed_up_bullet"),
        Actor("speed_up_bullet_selected"),
    )

    def speed_up_bullet(order: int, be_selected: bool):
        Buff.speed_up_bullet_tuple[be_selected].pos = Buff.buff_pos[order - 1]
        Game.actors.append(Buff.speed_up_bullet_tuple[be_selected])  # register to draw
        if be_selected and Buff.check_enter() and not Game.buff_take_effect:
            Game.hero.upgrade_bullet(Speed_Up_Bullet_Bullets_Decorator)
            Game.buff_take_effect = True

    @staticmethod
    def check_enter():
        if keyboard.RETURN:
            Game.buff_pressdown = True
            return True
        return False


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
