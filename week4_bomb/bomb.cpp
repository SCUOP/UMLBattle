// bubble.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int x = 0, y = 0;
int w = 850, h = 850;
int r = 20;
int i = 0, j = 0;
int speed = 5;
bool Box[17][17];
int enemy[3][4];
int bomb = 0;
int bx, by;
int bombing = 0;
bool upflag = true;
bool downflag = true;
bool leftflag = true;
bool rightflag = true;

bool game = true;
bool success = false;

void KeyControl()
{
	char userkey = _getch();
	if (userkey == ' ' && bomb == 0 && bombing == 0)
	{
		bomb += 200;
	}
	if (GetAsyncKeyState(VK_UP) && ((x - 25) % 100 == 0) && upflag)   // Up
	{
		y -= speed;
		if (y <= r) y = r;
	}
	if (GetAsyncKeyState(VK_DOWN) && ((x - 25) % 100 == 0) && downflag) // Down
	{
		y += speed;
		if (y >= w - r) y = w - r;
	}
	if (GetAsyncKeyState(VK_LEFT) && ((y - 25) % 100 == 0) && leftflag) // Left
	{
		x -= speed;
		if (x <= r) x = r;
	}
	if (GetAsyncKeyState(VK_RIGHT) && ((y - 25) % 100 == 0) && rightflag) // Right
	{
		x += speed;
		if (x >= w - r) x = w - r;
	}
}

void EnemyMove()
{
	for (int i = 0; i < 3; i++)
	{
		if (enemy[i][3] == 1) {
			// up
			if (enemy[i][2] == 0)
			{
				if ((enemy[i][1] == 25 || ((enemy[i][1] - 25) % 50 == 0 && Box[(enemy[i][0] - 25) / 50][(enemy[i][1] - 75) / 50])))
					enemy[i][2] = rand() % 4;
				else if ((enemy[i][0] - 25) % 100 == 0)
				{
					enemy[i][1] -= 1;
					if (enemy[i][1] < 25)
						enemy[i][1] = 25;
				}
				else
					enemy[i][2] = rand() % 4;
			}
			// down
			else if (enemy[i][2] == 1)
			{
				if ((enemy[i][1] == 825 || ((enemy[i][1] - 25) % 50 == 0 && Box[(enemy[i][0] - 25) / 50][(enemy[i][1] + 25) / 50])))
					enemy[i][2] = rand() % 4;
				else if ((enemy[i][0] - 25) % 100 == 0)
				{
					enemy[i][1] += 1;
					if (enemy[i][1] > 825)
						enemy[i][1] = 825;
				}
				else
					enemy[i][2] = rand() % 4;
			}
			// left
			else if (enemy[i][2] == 2)
			{
				if ((enemy[i][0] == 25 || (enemy[i][0] - 25) % 50 == 0 && Box[(enemy[i][0] - 75) / 50][(enemy[i][1] - 25) / 50]))
					enemy[i][2] = rand() % 4;
				else if ((enemy[i][1] - 25) % 100 == 0)
				{
					enemy[i][0] -= 1;
					if (enemy[i][0] < 25)
						enemy[i][0] = 25;
				}
				else
					enemy[i][2] = rand() % 4;
			}
			// right
			else
			{
				if ((enemy[i][0] == 825 || ((enemy[i][0] - 25) % 50 == 0 && Box[(enemy[i][0] + 25) / 50][(enemy[i][1] - 25) / 50])))
					enemy[i][2] = rand() % 4;
				else if ((enemy[i][1] - 25) % 100 == 0)
				{
					enemy[i][0] += 1;
					if (enemy[i][0] > 825)
						enemy[i][0] = 825;
				}
				else
					enemy[i][2] = rand() % 4;
			}
		}
	}
}

void DrawMap()
{
	for (i = 0; i < 17; i++)
	{
		for (j = 0; j < 17; j++)
		{
			if (i % 2 != 0 && j % 2 != 0)
			{
				int left = (i) * 50;
				int top = (j) * 50;
				int right = left + 50;
				int bottom = top + 50;
				setfillcolor(RGB(190, 190, 190));
				setlinecolor(RGB(150, 150, 150));
				fillrectangle(left, top, right, bottom);
			}
			else if (Box[i][j])
			{
				int left_b = (i) * 50 + 5;
				int top_b = (j) * 50 + 5;
				int right_b = left_b + 40;
				int bottom_b = top_b + 40;
				if (top_b == y + r && x <= right_b && x >= left_b)
					downflag = false;
				if (left_b == x + r && y >= top_b && y <= bottom_b)
					rightflag = false;
				if (right_b == x - r && y >= top_b && y <= bottom_b)
					leftflag = false;
				if (bottom_b == y - r && x <= right_b && x >= left_b)
					upflag = false;
				setlinecolor(RGB(50, 50, 50));
				setfillcolor(RGB(50, 50, 50));
				fillrectangle(left_b, top_b, right_b, bottom_b);
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (enemy[i][3] == 1) 
		{
			setlinecolor(BLUE);
			setfillcolor(BLUE);
			fillcircle(enemy[i][0], enemy[i][1], r);
			setlinecolor(RED);
			setfillcolor(RED);
		}
	}
}

void CreateBox()
{
	for (i = 0; i < 17; i++)
	{
		for (j = 0; j < 17; j++)
		{
			if ((i + j) % 2 != 0 && rand() % 3 == 0)
				Box[i][j] = true;
			else Box[i][j] = false;

			// Prevent the ball’s birth position from being blocked
			if (i * 50 <= x + 25 && i * 50 >= x - 75 && j * 50 <= y + 25 && j * 50 >= y - 75)
			{
				Box[i][j] = false;
			}
		}
	}
}

void CreateEnemy()
{
	for (int i = 0; i < 3; i++)
	{
		int m, n;
		while(true) 
		{
			m = rand() % 9 * 100 + r + 5;
			n = rand() % 9 * 100 + r + 5;
			if (m != x || n != y)
				break;
		}
		// x
		enemy[i][0] = m;
		// y
		enemy[i][1] = n;
		// 0-up, 1-down, 2-left, 3-right
		enemy[i][2] = rand() % 4;
		// live
		enemy[i][3] = 1;
	}
}

void CreateBomb()
{
	if (bomb == 200)
	{
		bx = x;
		by = y;
		bx -= ((x - 25) % 50);
		by -= ((y - 25) % 50);
	}
	if (bomb > 0)
	{
		--bomb;
		if (bomb == 0)
		{
			bombing = 100;
		}
		else
		{
			setfillcolor(BLACK);
			setlinecolor(BLACK);
			fillcircle(bx, by, r);
			setlinecolor(RED);
			setfillcolor(RED);
		}
	}
}

void Bombing() 
{
	if (bombing > 0)
	{
		--bombing;
		setfillcolor(BLACK);
		setlinecolor(BLACK);
		fillrectangle(bx - 75, by - 25, bx + 75, by + 25);
		fillrectangle(bx - 25, by - 75, bx + 25, by + 75);
		setlinecolor(RED);
		setfillcolor(RED);
		int i = (bx - 25) / 50;
		int j = (by - 25) / 50;
		if (i > 0) 
			Box[i - 1][j] = false;
		if (j > 0)
			Box[i][j - 1] = false;
		Box[i][j] = false;
		if (j + 1 < 17)
			Box[i][j + 1] = false;
		if (i + 1 < 17) 
			Box[i + 1][j] = false;

		// TODO: Destroy enemies

		// game over
		// TODO: collide with enemies
		if ((x - r <= bx + 75 && x + r >= bx - 75 && y - r <= by + 25 && y + r >= by - 25) || (x - r <= bx + 25 && x + r >= bx - 25 && y - r <= by + 75 && y + r >= by - 75))
			game = false;
	}
}

// TODO: success

int main()
{
	initgraph(w, h);	// Create screen
	setbkcolor(RGB(0, 120, 0));
	cleardevice();

	srand((unsigned int)time(NULL));

	x = rand() % 9 * 100 + r + 5; //  random position
	y = rand() % 9 * 100 + r + 5;

	CreateBox();
	CreateEnemy();

	BeginBatchDraw();
	while (game == true)
	{
		upflag = true, downflag = true, leftflag = true, rightflag = true;

		Bombing();

		DrawMap();  //  create map

		setfillcolor(RED);

		EnemyMove();

		if (_kbhit())
			KeyControl();

		CreateBomb();

		fillcircle(x, y, r);

		FlushBatchDraw();
		Sleep(10);
		cleardevice();

		enemy[0][3] = 0;
	}

	settextstyle(50, 0, _T("宋体"));
	if (success == true)
		outtextxy(300, 400, _T("SUCCESS"));
	else
		outtextxy(300, 400, _T("GAME OVER"));
	FlushBatchDraw();
	cleardevice();
	while (true);
	_getch();
	closegraph();
	return 0;
}

