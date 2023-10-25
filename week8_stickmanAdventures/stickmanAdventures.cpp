#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <graphics.h>
#include "conio.h"
#include "EasyXPng.h"
#include <vector>
#include <time.h>
#pragma comment(lib, "Winmm.lib")
using namespace std;

#define WIDTH  800
#define HEIGHT 600

void PlayMusicOnce(TCHAR fileName[80]) // 播放一次音乐函数
{
    TCHAR cmdString1[50];
    _stprintf(cmdString1, _T("open %s alias tmpmusic"), fileName); // 生成命令字符串
    mciSendString(_T("close tmpmusic"), NULL, 0, NULL);            // 先把前面一次的音乐关闭
    mciSendString(cmdString1, NULL, 0, NULL);                      // 打开音乐
    mciSendString(_T("play tmpmusic"), NULL, 0, NULL);             // 仅播放一次
}

class Land {
public:
	int x;
	bool exsit;
	bool enemy;
	bool star;
	void update()
	{
		x -= 5;
		if (x <= -100)
		{
			exsit = true;
			enemy = false;
			star = false;
			int r = rand() % 12;
			if (r == 0 || r == 1 || r == 2)
				enemy = true;
			else if (r == 3 || r == 4)
				star = true;
			else if (r == 5 || r == 6)
				;
			else
				exsit = false;
			x = 800;
		}
	}
};

int dx_1 = 0, dx_2 = 0, vy = 0, g = 1, y;
vector<Land> lands;
IMAGE im_bk, im_bk2, im_land, im_jump, img[8], im_star, im_enemy;

int game;

bool isCollide(int objx, int objy, int objw, int objh)
{
	int centerx = WIDTH / 4 + img[0].getwidth() / 2;
	int centery = y + img[0].getheight() / 2;
	if (abs(objx - centerx) <= objw * 0.5 && abs(objy - centery) <= objh * 0.5)
		return 1;
    else
		return 0;
}

bool isOnLand(Land land, int playery)
{
	int playerLeft = WIDTH / 4;
	int playerRight = playerLeft + 100;
	int landxLeft = land.x;
	int landxRight = land.x + 100;
	if (landxLeft - playerLeft <= 60 && playerRight - landxRight <= 60 && abs((HEIGHT * 2 / 3) - (y + img[0].getheight())) <= 2)
	{
		return true;
	}
	return false;
}

int main()
{
	srand(time(0));
	initgraph(WIDTH, HEIGHT);

	int i, j;
	int height;
	int bk_index = 1;
	TCHAR filename[50];

	for (i = 0; i < 8; i++)
	{
		_stprintf_s(filename, _T("material\\runright%d.png"), i);
		loadimage(&img[i], filename);
	}
	height = img[0].getheight();

	loadimage(&im_bk, _T("material\\landscape1.png"));
	loadimage(&im_bk2, _T("material\\landscape2.png"));
	loadimage(&im_land, _T("material\\land.png"));
	loadimage(&im_star, _T("material\\star.png"));
	loadimage(&im_enemy, _T("material\\bat.png"));
	loadimage(&im_jump, _T("material\\jumpright.png"));

	i = 0;
	BeginBatchDraw();
	y = HEIGHT * 2 / 3 - height;

	for (int k = 0; k < 9; k++)
	{
		Land land;
		land.x = k * 100;
		land.exsit = true;
		land.enemy = false;
		land.star = false;
		if (k == 6 || k == 7 || k == 8)
			// 三分之二的概率
			if (rand() % 3 == 0)
				;
			else
				land.exsit = false;
		lands.push_back(land);
	}
	game = true;
	while (game)
	{
		putimage(dx_1, -150 + HEIGHT * 2 / 3 - height - y, &im_bk);
		putimage(dx_1 + 2000, -150 + HEIGHT * 2 / 3 - height - y, &im_bk2);
		dx_1 -= 2;
		if (dx_1 <= -2000)
		{
			dx_1 = 0;
			bk_index++;
			if (bk_index == 10)
				bk_index = 1;
			_stprintf_s(filename, _T("material\\landscape%d.png"), bk_index);
			loadimage(&im_bk, filename);
			_stprintf_s(filename, _T("material\\landscape%d.png"), (bk_index + 1) % 9 + 1);
			loadimage(&im_bk2, filename);
		}

		bool onLand = false;
		for (j = 0; j < 9; j++)
		{
			if (lands[j].exsit)
			{
				putimage(lands[j].x, HEIGHT * 2 / 3 + HEIGHT * 2 / 3 - height - y, &im_land);
				if(isOnLand(lands[j], y))
					onLand = true;
			}
			else
			{
				if(lands[j].x <= 700)
					lands[(j + 1) % 9].exsit = true;
			}
			if (lands[j].enemy)
			{
				putimagePng(lands[j].x + im_land.getwidth() / 2, HEIGHT * 2 / 3 + HEIGHT * 2 / 3 - height - y - im_enemy.getheight(), &im_enemy);
				if (lands[j].x <= 700)
				{
					lands[(j + 1) % 9].enemy = false;
					lands[(j + 1) % 9].exsit = true;
				}
				if (isCollide(lands[j].x + im_land.getwidth() / 2 + im_enemy.getwidth() / 2, HEIGHT * 2 / 3 + HEIGHT * 2 / 3 - height - y - im_enemy.getheight(), im_enemy.getwidth(), im_enemy.getheight()))
				{
		            TCHAR filename[50];
					_stprintf(filename, _T("material\\warning.mp3"));
					PlayMusicOnce(filename);
					game = false;
				}
			}
			if (lands[j].star)
			{
				putimagePng(lands[j].x + im_land.getwidth() / 2, HEIGHT * 2 / 3 + HEIGHT * 2 / 3 - height - y - im_star.getheight(), &im_star);
				if (isCollide(lands[j].x + im_land.getwidth() / 2 + im_star.getwidth() / 2, HEIGHT * 2 / 3 + HEIGHT * 2 / 3 - height - y - im_star.getheight(), im_star.getwidth(), im_star.getheight()))
				{
			        TCHAR filename[50];
			        _stprintf(filename, _T("material\\success.mp3"));
					PlayMusicOnce(filename);
					lands[j].star = false;
				}
			}
			lands[j].update();
		}

		if (i > 7) i = 0;
		if (onLand)
			putimagePng(WIDTH / 4, HEIGHT * 2 / 3 - height, &img[i]);
		else
			putimagePng(WIDTH / 4, HEIGHT * 2 / 3 - height, &im_jump);

		i++;

		if (_kbhit() && onLand)
		{
			char input = _getch();
			if (input == ' ')
			{
				vy = -15;
				onLand = false;
			}
		}

		vy = vy + g;
		y = y + vy;

		if (onLand)
		{
			vy = 0;
			y = HEIGHT * 2 / 3 - height;
		}

		if (y >= HEIGHT * 2 / 3 - height + 100)
			game = false;

		Sleep(50);
		FlushBatchDraw();
	}
	setbkmode(TRANSPARENT);
    settextstyle(50, 0, _T("宋体"));
    outtextxy(WIDTH / 2 - 100, HEIGHT / 2 - 50, _T("GAME OVER"));
	FlushBatchDraw();
	cleardevice();
	while (true);
	_getch();
	return 0;
}


