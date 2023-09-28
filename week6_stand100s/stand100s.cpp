#include "stdafx.h"
#pragma warning(default: 4996)

#include <iostream>
#include <graphics.h>  
#include <conio.h>
#include <time.h>
#include "EasyXPng.h"  // 
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
		for (int i = 0; i <= life; i++)
		{
			putimagePng(im_blood.getwidth() / 2 , im_blood.getheight() / 2, &im_blood);
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
		if (x + radius > rocket.x - rocket.width / 2 + 5 && x - radius < rocket.x + rocket.width / 2 - 5 && y + radius > rocket.y - rocket.height / 2 + 5 && y - radius < rocket.y + rocket.height / 2 - 5) // +-5为了实现更加内部的碰撞效果
		{
			exist = false;
			boom = 50;
			rocket.life--;
		}
	}
};

IMAGE im_bk, im_bullet, im_rocket, im_blowup, im_blood;  // 	
Bullet bullet[MaxBulletNum]; // 
Rocket rocket;  // 
int bulletNum = 0; // 

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

	initgraph(WIDTH, HEIGHT); //
	BeginBatchDraw(); // 
}

void show()  // 
{
	putimage(0, 0, &im_bk);	// 
	rocket.draw();  // 
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

int main() // 
{
	startup();  // 
	while (rocket.life != 0)  // 
	{
		show();  // 
		//std::cout << rocket.life << std::endl;
		updateWithoutInput();  //
		updateWithInput();  // 
	}
	return 0;
}