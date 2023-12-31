//#define  _CRT_SECURE_NO_WARNINGS
//#include <graphics.h>
//#include <conio.h>
//#include <time.h>
//#include "EasyXPng.h"
//#include "Timer.h"
//#include <vector>
//using namespace std;
//#pragma comment(lib, "Winmm.lib")
//
//#define WIDTH 800
//#define HEIGHT 600
//
//void PlayMusicOnce(TCHAR fileName[80]) // 播放一次音乐函数
//{
//    TCHAR cmdString1[50];
//    _stprintf(cmdString1, _T("open %s alias tmpmusic"), fileName); // 生成命令字符串
//    mciSendString(_T("close tmpmusic"), NULL, 0, NULL);            // 先把前面一次的音乐关闭
//    mciSendString(cmdString1, NULL, 0, NULL);                      // 打开音乐
//    mciSendString(_T("play tmpmusic"), NULL, 0, NULL);             // 仅播放一次
//}
//
//enum PlayerStatus // 枚举类型，游戏角色所有的可能状态
//{
//    standleft, standright, runleft, runright, jumpleft, jumpright, die
//};
//
//class Land // 地面类
//{
//public:
//    IMAGE im_land;                 // 地面图像
//    float left_x, right_x, top_y;  // 用来刻画一块地面的左、右、上坐标
//    float land_width, land_height; // 一块地面图像的宽度、高度
//
//    void initialize() // 初始化
//    {
//        loadimage(&im_land, _T("material\\land.png")); // 导入地面图片
//        land_width = im_land.getwidth();     // 获得地面图像的宽、高
//        land_height = im_land.getheight();
//        left_x = WIDTH / 2; // land初始化在画面正中间，正好就在玩家角色脚底下
//        right_x = left_x + land_width;
//        top_y = HEIGHT / 2;
//    }
//
//    void draw(float px, float py) // 显示相关信息
//    {
//        putimage(left_x - px, top_y - py, &im_land); // 角色不动，绘制地面有一个偏移量
//    }
//};
//
//class Enemy // 敌人
//{
//public:
//    IMAGE im_enemy;
//    int x, y;
//    int width, height;
//    int min, max;
//    int vx;
//
//    void initialize() // 初始化
//    {
//        loadimage(&im_enemy, _T("material\\bat.png")); // 导入敌人-蝙蝠-图片
//        width = im_enemy.getwidth();   // 获得图像的宽、高
//        height = im_enemy.getheight();
//    }
//
//    void draw(float px, float py) // 显示相关信息
//    {
//        putimagePng(x - width / 2 - px, y - height / 2 - py, &im_enemy);
//    }
//
//    void update() // 敌人在一定范围内，左右移动
//    {
//        x += vx;
//        if (x > max || x < min)
//            vx = -vx;
//    }
//};
//
//class Scene // 游戏场景类
//{
//public:
//    IMAGE im_bk;           // 背景图像
//    IMAGE im_star;         // 终点
//    vector<Land> lands;    // 地面
//    vector<Enemy> enemies; // 敌人
//    int level;             // 关卡
//    int lastlevel;
//
//    void draw(float px, float py) // 显示相关信息
//    {
//        // 角色不动，绘制背景有一个偏移量
//        // 背景偏移量/20，就形成了有深度差的前后景的视觉效果
//        putimage(-px / 20, -100 - py / 20, &im_bk); // 显示背景
//
//        for (int i = 0; i < lands.size(); i++)
//            lands[i].draw(px, py); // 绘制所有地面
//
//        for (int i = 0; i < enemies.size(); i++)
//            enemies[i].draw(px, py); // 绘制所有敌人
//
//        // 终点
//        putimagePng(lands[lands.size() - 1].left_x + im_star.getwidth() - px,
//            lands[lands.size() - 1].top_y - im_star.getheight() - py, &im_star);
//
//        // 显示这是第几关
//        TCHAR s[20];                         // 字符串
//        setbkmode(TRANSPARENT);              // 文字透明显示
//        _stprintf(s, _T("level:%d"), level); // 生成文字字符串
//        settextstyle(50, 0, _T("宋体"));
//        outtextxy(0, 30, s);      // 输出文字
//    }
//
//    void initialize() // 初始化
//    {
//        TCHAR filename[50];
//        int i = level % 9 + 1;
//        _stprintf(filename, _T("material\\landscape%d.png"), i);
//        loadimage(&im_bk, filename);
//        loadimage(&im_star, _T("material\\star.png"));
//
//        if (lands.size() == 0)
//        {
//            level = 1;
//            lastlevel = 1;
//        }
//
//        if (lands.size() == 0 || level > lastlevel)
//        {
//            lands.clear(); // 重置板
//            // 前后迭代画板;
//            Land beforeLand;
//            beforeLand.initialize();
//            lands.push_back(beforeLand);
//
//            for (int i = 1; i < 10 + level; i++)
//            {
//                Land afterLand;
//                afterLand.initialize();
//                int skipLand = rand() % 10;
//                if (skipLand > level)
//                    afterLand.left_x = beforeLand.left_x + afterLand.land_width;
//                else // 空一格板子
//                    afterLand.left_x = beforeLand.left_x + 2 * afterLand.land_width;
//
//                int staggered = rand() % 10;
//                if (staggered > level) // 交错
//                    afterLand.top_y = beforeLand.top_y;
//                else
//                {
//                    int upordown = rand() % 2;
//                    if (upordown == 0) //0上 1下
//                        afterLand.top_y = WIDTH / 2 - HEIGHT / 10;
//                    else
//                        afterLand.top_y = WIDTH / 2 + HEIGHT / 10;
//                }
//
//                afterLand.right_x = afterLand.left_x + afterLand.land_width;
//
//                lands.push_back(afterLand);
//                beforeLand = afterLand;
//            }
//
//            enemies.clear();
//            // 敌人数目
//            int numEnemy = level - 1;
//            int enemyIndex = lands.size() / (numEnemy + 1);
//            for (int i = 1; i <= numEnemy; i++)
//            {
//                Enemy enemy;
//                enemy.initialize();
//                int landIndex = i * enemyIndex;
//                enemy.x = lands[landIndex].left_x + lands[landIndex].land_width / 2;
//                enemy.y = lands[landIndex].top_y - enemy.height;
//                // 根据level加强敌人
//                enemy.min = enemy.x - (rand() % level + 2) * enemy.width;
//                enemy.max = enemy.x + (rand() % level + 2) * enemy.width;
//                enemy.vx = 2 + rand() % level;
//
//                enemies.push_back(enemy);
//            }
//        }
//    }
//};
//
//class Player // 玩家控制的游戏角色类
//{
//public:
//    IMAGE im_show;              // 当前时刻要显示的图像
//    IMAGE im_standright;        // 向右站立图像
//    IMAGE im_standleft;         // 向左站立图像
//    IMAGE im_jumpright;         // 向右方向跳跃图像
//    IMAGE im_jumpleft;          // 向左方向跳跃图像
//    vector<IMAGE> ims_runright; // 向右奔跑的图像序列
//    vector<IMAGE> ims_runleft;  // 向左奔跑的图像序列
//    int animId;                 // 用于循环动画播放的id
//    PlayerStatus playerStatus;  // 当前的状态
//    float x_left, y_bottom;     // 这两个坐标，因为只要用这两个和地面碰撞就行了
//    float vx, vy;               // 速度
//    float gravity;              // 重力加速度
//    float width, height;        // 图片的宽度、高度
//
//    void draw() // 显示相关信息
//    {
//        // 角色在游戏中心
//        putimagePng(WIDTH / 2, HEIGHT / 2 - height, &im_show);
//    }
//
//    void initialize() // 初始化
//    {
//        ims_runleft.clear(); // 先清空掉vector
//        ims_runright.clear();
//        loadimage(&im_standright, _T("material\\standright.png")); // 导入向右站立图片
//        loadimage(&im_standleft, _T("material\\standleft.png"));   // 导入向左站立图片
//        loadimage(&im_jumpright, _T("material\\jumpright.png"));   // 导入向右方向跳跃图像
//        loadimage(&im_jumpleft, _T("material\\jumpleft.png"));     // 导入向左方向跳跃图像
//
//        playerStatus = standright;        // 初始为向右站立的游戏状态
//        im_show = im_standright;          // 初始显示向右站立的图片
//        width = im_standright.getwidth(); // 获得图像的宽、高，所有动画图片大小一样
//        height = im_standright.getheight();
//
//        TCHAR filename[50];
//        for (int i = 0; i <= 7; i++) // 把向右奔跑的八张图片对象添加到ims_runright中
//        {
//            _stprintf(filename, _T("material\\runright%d.png"), i);
//            IMAGE im;
//            loadimage(&im, filename);
//            ims_runright.push_back(im);
//        }
//        for (int i = 0; i <= 7; i++) // 把向左奔跑的八张图片对象添加到ims_runright中
//        {
//            _stprintf(filename, _T("material\\runleft%d.png"), i);
//            IMAGE im;
//            loadimage(&im, filename);
//            ims_runleft.push_back(im);
//        }
//
//        animId = 0; // 动画id开始设为0
//
//        updateXY(WIDTH / 2, HEIGHT / 2); // 开始将角色放在画面中间
//        vx = 10;                         // 水平方向初速度
//        vy = 0;                          // 竖直方向速度初始为0
//        gravity = 3;                     // 设定重力加速度
//    }
//
//    void updateXY(float mx, float my) // 根据输入，更新玩家坐标
//    {
//        x_left = mx;
//        y_bottom = my;
//    }
//
//    void runRight(Scene& scene) // 游戏角色向右奔跑
//    {
//        x_left += vx;                         // 横坐标增加，向右移动
//        if (isNotOnAllLands(scene.lands, vy)) // 移动后不在任何一块地面上了
//        {
//            im_show = im_jumpright;   // 切换到向右起跳图片
//            playerStatus = jumpright; // 切换到向右起跳状态
//            return;
//        }
//
//        if (playerStatus == jumpleft || playerStatus == jumpright) // 如果是起跳状态
//        {
//            im_show = im_jumpright; // 改变造型为向右起跳造型
//        }
//        else
//        {
//            if (playerStatus != runright) // 如果之前角色状态不是向右奔跑
//            {
//                playerStatus = runright; // 切换为向右奔跑状态
//                animId = 0;              // 动画播放id初始化为0
//            }
//            else // 表示之前就是向右奔跑状态了
//            {
//                animId++;                          // 动画图片开始切换
//                if (animId >= ims_runright.size()) // 循环切换
//                    animId = 0;
//            }
//            im_show = ims_runright[animId]; // 设置要显示的对应图片
//        }
//    }
//
//    void runLeft(Scene& scene) // 游戏角色向左奔跑
//    {
//        x_left -= vx;                         // 横坐标减少，向左移动
//        if (isNotOnAllLands(scene.lands, vy)) // 移动后不在任何一块地面上了
//        {
//            im_show = im_jumpleft;   // 切换到向左起跳图片
//            playerStatus = jumpleft; // 切换到向左起跳状态
//            return;
//        }
//
//        if (playerStatus == jumpleft || playerStatus == jumpright) // 如果是起跳状态
//        {
//            im_show = im_jumpleft; // 改变造型为向左起跳造型
//        }
//        else
//        {
//            if (playerStatus != runleft) // 如果之前角色状态不是向左奔跑
//            {
//                playerStatus = runleft; // 切换为向左奔跑状态
//                animId = 0;             // 动画播放id初始化为0
//            }
//            else // 之前就是向左奔跑状态了
//            {
//                animId++;                         // 动画图片开始切换
//                if (animId >= ims_runleft.size()) // 循环切换
//                    animId = 0;
//            }
//            im_show = ims_runleft[animId]; // 设置要显示的对应图片
//        }
//    }
//
//    void standStill() // 游戏角色默认为向左或向右静止站立
//    {
//        if (playerStatus == runleft || playerStatus == standleft)
//            im_show = im_standleft;
//        else if (playerStatus == runright || playerStatus == standright)
//            im_show = im_standright;
//    }
//
//    void beginJump() // 按下w或向上方向键后，游戏角色跳跃的处理
//    {
//        if (playerStatus != jumpleft && playerStatus != jumpright) // 已经在空中的话，不要起跳
//        {
//            if (playerStatus == runleft || playerStatus == standleft) // 起跳前是向左跑或向左站立状态
//            {
//                im_show = im_jumpleft;   // 切换到向左起跳图片
//                playerStatus = jumpleft; // 切换到向左起跳状态
//            }
//            else if (playerStatus == runright || playerStatus == standright) // 起跳前是向右跑或向右站立状态
//            {
//                im_show = im_jumpright;   // 切换到向右起跳图片
//                playerStatus = jumpright; // 切换到向右起跳状态
//            }
//            vy = -30; // 给角色一个向上的初速度
//        }
//    }
//
//    int isCollide(Enemy& enemy) // 判断角色是否和敌人碰撞，如果是返回1，否则返回0
//    {
//        float x_center = x_left + width / 2;
//        float y_center = y_bottom - height / 2;
//        if (abs(enemy.x - x_center) <= enemy.width * 0.5 && abs(enemy.y - y_center) <= enemy.height * 0.5)
//            return 1;
//        else
//            return 0;
//    }
//
//    // 判断游戏角色是否正站在这块地面上，如果是的话返回1，否则返回0
//    int isOnLand(Land& land, float ySpeed)
//    {
//        float x_right = x_left + width;
//        // 判断是否站在地面上，还需要考虑player的y方向速度情况，速度过快有可能直接穿透地面
//        if (ySpeed <= 0) // y轴方向速度小于0，表示正在向上运动，不需要考虑速度的影响
//            ySpeed = 0;
//        if (land.left_x - x_left <= width * 0.6 && x_right - land.right_x <= width * 0.6 && abs(y_bottom - land.top_y) <= 5 + ySpeed)
//            return 1;
//        else
//            return 0;
//    }
//
//    int isNotOnAllLands(vector<Land>& lands, float speed) // 判断玩家是否不在所有的地面上
//    {
//        for (int i = 0; i < lands.size(); i++)
//        {
//            if (isOnLand(lands[i], speed))
//                return 0; // 在任何一块地面上，返回0
//        }
//        return 1; // 不在所有地面上，返回1
//    }
//
//    void updateYcoordinate(Scene& scene) // x坐标是按键盘控制的，而y坐标是每帧自动更新的
//    {
//        if (playerStatus == jumpleft || playerStatus == jumpright) // 当前是在空中跳跃状态
//        {
//            vy += gravity; // y方向速度受重力影响变化
//            y_bottom += vy; // y方向位置受速度影响变化
//            for (int i = 0; i < scene.lands.size(); i++) // 对所有地面遍历
//            {
//                if (isOnLand(scene.lands[i], vy)) // 当火柴人正好站在一个地面上时
//                {
//                    y_bottom = scene.lands[i].top_y; // 保证正好落在地面上
//                    if (playerStatus == jumpleft)    // 向左跳，落地后切换到向左站立方向
//                        playerStatus = standleft;
//                    if (playerStatus == jumpright) // 向右跳，落地后切换到向右站立方向
//                        playerStatus = standright;
//                    break; // 跳出循环，不需要再对其他地面判断了
//                }
//            }
//        }
//    }
//};
//
//// 一些全局变量
//Player player; // 定义玩家控制的游戏角色对象
//Scene scene;   // 定义场景全局对象
//Timer timer;   // 用于精确延时
//
//void startup() // 初始化
//{
//    scene.level = 3;
//    Land land;
//    scene.lands.push_back(land);
//    srand(time(0));           // 初始化随机数种子
//    scene.initialize();       // 场景初始化
//    player.initialize();      // 玩家角色初始化
//    initgraph(WIDTH, HEIGHT); // 新开一个画面
//    BeginBatchDraw();         // 开始批量绘制
//}
//
//void startup2() // 初始化
//{
//
//    srand(time(0));           // 初始化随机数种子
//    scene.initialize();       // 场景初始化
//    player.initialize();      // 玩家角色初始化
//    initgraph(WIDTH, HEIGHT); // 新开一个画面
//    BeginBatchDraw();         // 开始批量绘制
//}
//
//void show() // 显示
//{
//    scene.draw(player.x_left - WIDTH / 2, player.y_bottom - HEIGHT / 2); // 显示场景相关信息
//    player.draw();                                                       // 显示玩家相关信息
//    FlushBatchDraw();                                                    // 批量绘制
//    timer.Sleep(50);                                                     // 暂停若干毫秒
//}
//
//void show2() // 显示
//{
//    scene.draw(player.x_left - WIDTH / 2, player.y_bottom - HEIGHT / 2); // 显示场景相关信息
//    player.draw();                                                       // 显示玩家相关信息
//    FlushBatchDraw();                                                    // 批量绘制
//    timer.Sleep(50);                                                     // 暂停若干毫秒
//}
//
//void updateWithoutInput() // 和输入无关的更新
//{
//    player.updateYcoordinate(scene); // 游戏角色y坐标是每帧自动更新的
//
//    int landSize = scene.lands.size();
//    // 到达终点
//    if (player.x_left > scene.lands[landSize - 1].left_x && abs(player.y_bottom - scene.lands[landSize - 1].top_y) <= 2)
//    {
//        TCHAR filename[50];
//        _stprintf(filename, _T("material\\success.mp3"));
//        PlayMusicOnce(filename);
//        scene.lastlevel = scene.level;
//    }
//    else if (player.y_bottom > 1.5 * HEIGHT) // 角色掉落
//    {
//        scene.lastlevel = scene.level;
//        scene.initialize();            // 重置
//        player.initialize();
//    }
//
//    for (int i = 0; i < scene.enemies.size(); i++) //碰撞检测
//    {
//        scene.enemies[i].update();
//        if (player.isCollide(scene.enemies[i]))
//        {
//            TCHAR filename[50];
//            _stprintf(filename, _T("material\\warning.mp3"));
//            PlayMusicOnce(filename);
//            scene.lastlevel = scene.level;
//            scene.initialize();
//            player.initialize();
//        }
//    }
//}
//
//void updateWithoutInput2() // 和输入无关的更新
//{
//    player.updateYcoordinate(scene); // 游戏角色y坐标是每帧自动更新的
//
//    int landSize = scene.lands.size();
//    // 到达终点
//    if (player.x_left > scene.lands[landSize - 1].left_x && abs(player.y_bottom - scene.lands[landSize - 1].top_y) <= 2)
//    {
//        TCHAR filename[50];
//        _stprintf(filename, _T("material\\success.mp3"));
//        PlayMusicOnce(filename);
//        scene.lastlevel = scene.level;
//        scene.level++;
//        scene.initialize(); // 下一关 
//        player.initialize();
//    }
//    else if (player.y_bottom > 1.5 * HEIGHT) // 角色掉落
//    {
//        scene.lastlevel = scene.level;
//        scene.initialize();            // 重置
//        player.initialize();
//    }
//
//    for (int i = 0; i < scene.enemies.size(); i++) //碰撞检测
//    {
//        scene.enemies[i].update();
//        if (player.isCollide(scene.enemies[i]))
//        {
//            TCHAR filename[50];
//            _stprintf(filename, _T("material\\warning.mp3"));
//            PlayMusicOnce(filename);
//            scene.lastlevel = scene.level;
//            scene.initialize();
//            player.initialize();
//        }
//    }
//}
//
//void updateWithInput() // 和输入有关的更新
//{
//    player.standStill(); // 游戏角色默认为向左或向右静止站立
//
//    if (_kbhit()) // 当按键时，切换角色显示图片，更改位置
//    {
//        //if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) // 按下D键或右方向键
//        //    player.runRight(scene);
//        //else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) // 按下A键或左方向键
//        //    player.runLeft(scene);
//        char input = _getch();
//        if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W') || input == ' ') // 按下W键或上方向键
//            player.beginJump();
//    }
//    player.runRight(scene);
//}
//
//void updateWithInput2() // 和输入有关的更新
//{
//    player.standStill(); // 游戏角色默认为向左或向右静止站立
//
//    if (_kbhit()) // 当按键时，切换角色显示图片，更改位置
//    {
//        if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) // 按下D键或右方向键
//            player.runRight(scene);
//        else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) // 按下A键或左方向键
//            player.runLeft(scene);
//        char input = _getch();
//        if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W') || input == ' ') // 按下W键或上方向键
//            player.beginJump();
//    }
//}
//int tmain() // 主函数
//{
//    startup(); // 初始化
//    while (1)  // 一直循环
//    {
//        show();               // 显示
//        updateWithoutInput(); // 与输入无关的更新
//        updateWithInput();    // 与输入有关的更新
//        if (scene.level == 10)
//            break;
//    }
//    settextstyle(50, 0, _T("宋体"));
//    outtextxy(WIDTH / 2 - 100, HEIGHT / 2 - 50, _T("GAME OVER"));
//    FlushBatchDraw();
//    cleardevice();
//    while (true);
//    getchar();
//    return 0;
//}