import pgzrun  # 导入游戏库

WIDTH = 640  # 设置窗口的宽度
HEIGHT = 480  # 设置窗口的高度

backgroud = Actor("bg")
# 导入所有的分解动作图片，存在列表当中
AnimsDown = [
    Actor("gd0"),
    Actor("gd1"),
    Actor("gd2"),
    Actor("gd3"),
    Actor("gd4"),
    Actor("gd5"),
    Actor("gd6"),
    Actor("gd7"),
]
AnimsLeft = [
    Actor("gl0"),
    Actor("gl1"),
    Actor("gl2"),
    Actor("gl3"),
    Actor("gl4"),
    Actor("gl5"),
    Actor("gl6"),
    Actor("gl7"),
]
AnimsRight = [
    Actor("gr0"),
    Actor("gr1"),
    Actor("gr2"),
    Actor("gr3"),
    Actor("gr4"),
    Actor("gr5"),
    Actor("gr6"),
    Actor("gr7"),
]
AnimsUp = [
    Actor("gu0"),
    Actor("gu1"),
    Actor("gu2"),
    Actor("gu3"),
    Actor("gu4"),
    Actor("gu5"),
    Actor("gu6"),
    Actor("gu7"),
]

numAnims = 8  # 分解动作图片的张数
animIndex = 0  # 表示需要显示的动作图片的序号
animSpeed = 0  # 用于控制行走动画速度

player_x = WIDTH / 2  # 设置玩家的x坐标
player_y = HEIGHT / 2  # 设置玩家的y坐标
for i in range(numAnims):
    AnimsLeft[i].x = player_x  # 设置所有分解动作图片的x坐标
    AnimsLeft[i].y = player_y  # 设置所有分解动作图片的y坐标
    AnimsDown[i].x = player_x  # 设置所有分解动作图片的x坐标
    AnimsDown[i].y = player_y  # 设置所有分解动作图片的y坐标
    AnimsUp[i].x = player_x  # 设置所有分解动作图片的x坐标
    AnimsUp[i].y = player_y  # 设置所有分解动作图片的y坐标
    AnimsRight[i].x = player_x  # 设置所有分解动作图片的x坐标
    AnimsRight[i].y = player_y  # 设置所有分解动作图片的y坐标

nowAnims = AnimsRight[0]


def draw():  # 绘制模块，每帧重复执
    backgroud.draw()
    nowAnims.draw()  # 绘制玩家当前分解动作图片


def update():  # 更新模块，每帧重复操作
    global animIndex, player_x, player_y, animSpeed, nowAnims
    if keyboard.right:  # 如果按下键盘右键
        player_x += 5  # 角色向右移动
        for i in range(numAnims):  # 所有分解动作图片更新x坐标
            AnimsLeft[i].x = player_x
            AnimsRight[i].x = player_x
            AnimsDown[i].x = player_x
            AnimsUp[i].x = player_x
        if player_x >= WIDTH:  # 角色走到最右边
            player_x = 0  # 再从最左边出现
        animSpeed += 1  # 用于控制动作动画速度
        if animSpeed % 5 == 0:  # 动作动画速度是移动速度的1/5
            animIndex += 1  # 每一帧分解动作图片序号加1
            if animIndex >= numAnims:  # 放完最后一个分解动作图片了
                animIndex = 0  # 再变成第一张分解动作图片
        nowAnims = AnimsRight[animIndex]
    elif keyboard.left:  # 如果按下键盘左键
        player_x -= 5  # 角色向左移动
        for i in range(numAnims):  # 所有分解动作图片更新x坐标
            AnimsLeft[i].x = player_x
            AnimsRight[i].x = player_x
            AnimsDown[i].x = player_x
            AnimsUp[i].x = player_x
        if player_x <= 0:  # 角色走到最左边
            player_x = WIDTH  # 再从最右边出现
        animSpeed += 1  # 用于控制动作动画速度
        if animSpeed % 5 == 0:  # 动作动画速度是移动速度的1/5
            animIndex += 1  # 每一帧分解动作图片序号加1
            if animIndex >= numAnims:  # 放完最后一个分解动作图片了
                animIndex = 0  # 再变成第一张分解动作图片
        nowAnims = AnimsLeft[animIndex]
    elif keyboard.up:  # 如果按下键盘上键
        player_y -= 5  # 角色向上移动
        for i in range(numAnims):  # 所有分解动作图片更新y坐标
            AnimsLeft[i].y = player_y
            AnimsRight[i].y = player_y
            AnimsDown[i].y = player_y
            AnimsUp[i].y = player_y
        if player_y <= 0:  # 角色走到最上边
            player_y = HEIGHT  # 再从最下边出现
        animSpeed += 1  # 用于控制动作动画速度
        if animSpeed % 5 == 0:  # 动作动画速度是移动速度的1/5
            animIndex += 1  # 每一帧分解动作图片序号加1
            if animIndex >= numAnims:  # 放完最后一个分解动作图片了
                animIndex = 0  # 再变成第一张分解动作图片
        nowAnims = AnimsUp[animIndex]
    elif keyboard.down:  # 如果按下键盘右键
        player_y += 5  # 角色向下移动
        for i in range(numAnims):  # 所有分解动作图片更新x坐标
            AnimsLeft[i].y = player_y
            AnimsRight[i].y = player_y
            AnimsDown[i].y = player_y
            AnimsUp[i].y = player_y
        if player_y >= HEIGHT:  # 角色走到最下边
            player_y = 0  # 再从最上边出现
        animSpeed += 1  # 用于控制动作动画速度
        if animSpeed % 5 == 0:  # 动作动画速度是移动速度的1/5
            animIndex += 1  # 每一帧分解动作图片序号加1
            if animIndex >= numAnims:  # 放完最后一个分解动作图片了
                animIndex = 0  # 再变成第一张分解动作图片
        nowAnims = AnimsDown[animIndex]


pgzrun.go()  # 开始执行游戏
