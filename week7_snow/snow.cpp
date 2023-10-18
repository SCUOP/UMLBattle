#include <stdio.h>
#include <graphics.h>
#include "conio.h"
#include "EasyXPng.h"

#define WIDTH 787
#define HEIGHT 590
#define MAX_SNOW_NUM 50

int SNOW_NUM = 0;

class Snow
{
public:
	IMAGE im_snow;
	float x, y;
	float r;

	void draw()
	{
		putimagePng(x - r, y - r, &im_snow);
	}

	void update()
	{
		if (rand() % 2 == 0)
			x += 3;
		else
			x -= 3;

		y += 3;

		if (y > 590)
		{
			x = rand() % WIDTH;
			y = 0;
		}
	}
};

int main()
{
	IMAGE im_bk, im_snow;
	initgraph(WIDTH, HEIGHT);

	loadimage(&im_bk, _T("material\\bg.png"));
	loadimage(&im_snow, _T("material\\snow.png"));

	Snow snows[MAX_SNOW_NUM];
	snows[0].im_snow = im_snow;
	snows[0].r = im_snow.getwidth() / 2;
	snows[0].x = rand() % WIDTH;
	snows[0].y = 0;
	DWORD lasttime = GetTickCount();
	DWORD now = GetTickCount();
	BeginBatchDraw();
	while (true)
	{
		putimage(0, 0, &im_bk);

		for (int i = 0; i < SNOW_NUM + 1; i++)
		{
			snows[i].draw();
			snows[i].update();
		}

		if (SNOW_NUM + 1 < MAX_SNOW_NUM)
		{
			now = GetTickCount();
			if (now - lasttime > 300)
			{
				lasttime = now;
				SNOW_NUM++;
				snows[SNOW_NUM].im_snow = im_snow;
				snows[SNOW_NUM].r = im_snow.getwidth() / 2;
				snows[SNOW_NUM].x = rand() % WIDTH;
				snows[SNOW_NUM].y = 0;
			}
		}

		FlushBatchDraw();
		Sleep(20);
	}

	_getch();
	return 0;
}