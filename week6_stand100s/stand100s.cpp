#include "stdafx.h"
#pragma warning(default: 4996)

#include <iostream>
#include <graphics.h>  
#include <conio.h>
#include <time.h>
#include "EasyXPng.h"  // 
#pragma comment(lib,"winmm.lib")
#define  WIDTH 560 //
#define  HEIGHT 800 // 
#define	 MaxBulletNum 200  // 

void HideCursor(bool Visible)
{
	CONSOLE_CURSOR_INFO Cursor;
	Cursor.bVisible = !Visible;
	Cursor.dwSize = sizeof(Cursor);
	HANDLE Hand = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(Hand, &Cursor);
}

class Rocket  // 
{
public:
	IMAGE im_rocket, im_blood;             //
	float x, y;                  // 
	float width, height;         // 
	int life;

	void draw()                  //
	{
		putimagePng(x - width / 2, y - height / 2, &im_rocket);
		// 
		for (int i = 0; i < life; i++)
		{
			putimagePng((i + 1) * im_blood.getwidth(), im_blood.getheight(), &im_blood);
		}
	}

	void update(float mx, float my) // 
	{
		x = mx;
		y = my;
	}
};

class Bullet  // 
{
public:
	IMAGE im_bullet, im_blowup; // 
	float x, y; // 
	float vx, vy; // 
	float radius; //
	boolean exist;
	int boom;

	void draw()// 
	{
		if (exist == false && boom == 0)
			return;
		putimagePng(x - radius, y - radius, &im_bullet);
		if (boom != 0)
		{
			putimagePng(x - im_blowup.getwidth() / 2, y - im_blowup.getheight() / 2, &im_blowup);
			boom--;
		}
	}

	void update() // 
	{
		if (exist == false)
			return;
		x += vx;
		y += vy;
		if (x <= 0 || x >= WIDTH)
			vx = -vx;
		if (y <= 0 || y >= HEIGHT)
			vy = -vy;
	}

	void checkCollosion(Rocket &rocket)
	{
		if (exist == false)
			return;
		if (x + radius > rocket.x - rocket.width / 2 + 5 && x - radius < rocket.x + rocket.width / 2 - 5 && y + radius > rocket.y - rocket.height / 2 + 5 && y - radius < rocket.y + rocket.height / 2 - 5) // +-5 for better collosion
		{
			exist = false;
			boom = 50;
			rocket.life--;
			PlayMusicOnce((TCHAR*)_T("material\\boom.mp3"));
		}
	}

	void PlayMusicOnce(TCHAR fileName[80])
	{
		TCHAR cmdString1[50];
		swprintf_s(cmdString1, _T("open %s alias tmpmusic"), fileName);    // 生成命令字符串
		mciSendString(_T("close tmpmusic"), NULL, 0, NULL);                // 先把前面一次的音乐关闭
		mciSendString(cmdString1, NULL, 0, NULL);                          // 打开音乐
		mciSendString(_T("play tmpmusic"), NULL, 0, NULL);                 // 仅播放一次
	}
};

IMAGE im_bk, im_bullet, im_rocket, im_blowup, im_blood;  // 	
Bullet bullet[MaxBulletNum]; // 
Rocket rocket;  // 
int bulletNum = 0; // 
int standtime = 0;

void startup()  //  
{
	srand(time(0)); // 
	loadimage(&im_bk, _T("material\\background.png")); // 
	loadimage(&im_bullet, _T("material\\bullet.png")); // 
	loadimage(&im_rocket, _T("material\\rocket.png")); // 
	loadimage(&im_blowup, _T("material\\blowup.png")); // 
	loadimage(&im_blood, _T("material\\blood.png"));
	// 
	rocket.im_rocket = im_rocket;  // 
	rocket.width = im_rocket.getwidth(); // 
	rocket.height = im_rocket.getheight(); //
	rocket.life = 5;
	rocket.im_blood = im_blood;

	mciSendString(_T("open material\\bgm.mp3 alias bk"), NULL, 0, NULL);
	mciSendString(_T("play bk repeat"), NULL, 0, NULL);

	initgraph(WIDTH, HEIGHT); //
	setbkmode(TRANSPARENT);
	BeginBatchDraw(); // 
}

void show()  // 
{
	putimage(0, 0, &im_bk);	// 
	rocket.draw();  // 
	settextstyle(20, 0, _T("宋体"));
	TCHAR count_char[10];
	swprintf_s(count_char, _T("TIME: %d"), standtime);
	outtextxy(WIDTH - 100, 10, count_char);
	for (int i = 0; i < bulletNum; i++)
		bullet[i].draw();  // 
	FlushBatchDraw(); // 
	Sleep(10); // 
}

void updateWithoutInput() //
{
	static int lastSecond = 0; // 
	static int nowSecond = 0; // 
	static clock_t start = clock(); // 
	clock_t now = clock(); // 
	// 
	nowSecond = (int(now - start) / CLOCKS_PER_SEC);
	standtime = nowSecond;
	if (nowSecond == lastSecond + 2) //
	{
		lastSecond = nowSecond; // 
		// 
		if (bulletNum < MaxBulletNum)
		{
			bullet[bulletNum].x = WIDTH / 2; //
			bullet[bulletNum].y = 10;
			bullet[bulletNum].exist = true;
			bullet[bulletNum].boom = 0;
			float angle = (rand() / double(RAND_MAX) - 0.5) * 0.9 * PI;
			float scalar = 2 * rand() / double(RAND_MAX) + 2;
			bullet[bulletNum].vx = scalar * sin(angle); // 
			bullet[bulletNum].vy = scalar * cos(angle);
			bullet[bulletNum].im_bullet = im_bullet;  // 
			bullet[bulletNum].im_blowup = im_blowup;
			bullet[bulletNum].radius = im_bullet.getwidth() / 2; // 
		}
		bulletNum++; // 
	}

	for (int i = 0; i < bulletNum; i++) // 
	{
		bullet[i].update();
		bullet[i].checkCollosion(rocket);
	} // 
}

void updateWithInput()               // 
{
	MOUSEMSG m;		                 // 
	while (MouseHit())               // 
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_MOUSEMOVE)  // 		
			rocket.update(m.x, m.y); // 
	}
}

void endgame()
{
	settextstyle(50, 0, _T("宋体"));
	outtextxy(180, 350, _T("GAME OVER"));
	FlushBatchDraw();
	cleardevice();
	while (true);
	getchar();
}

int main() // 
{
	startup();  // 
	while (1)  // 
	{
		show();  // 
		if (rocket.life == 0)
			break;
		updateWithoutInput();  //
		updateWithInput();  // 
	}
	endgame();
	return 0;
}