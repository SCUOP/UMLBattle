
#include <stdio.h>
#include <graphics.h>
#include "conio.h"
#include "EasyXPng.h"

#define WIDTH  640
#define HEIGHT 480

int speed_1 = 2, speed_2 = 3;
int dx_1 = 0, dx_2 = 0;

int x = WIDTH / 4;
int cnt;

int main()
{
	IMAGE im_bk, im_house, im_hill;
	initgraph(WIDTH, HEIGHT);

	int i;
	TCHAR filename[20];
	IMAGE img[8];

	for (i = 0; i < 8; i++)
	{
		_stprintf_s(filename, _T("material\\d%d.png"), i);
		loadimage(&img[i], filename);
	}

	loadimage(&im_bk, _T("material\\bg0.png"));
	loadimage(&im_house, _T("material\\house.png"));
	loadimage(&im_hill, _T("material\\hill.png"));

	i = 0;
	BeginBatchDraw();
	while (1)
	{
		putimage(0, 0, dx_1, 300, &im_bk, WIDTH - dx_1, 0, SRCCOPY);
		putimage(dx_1, 0, WIDTH - dx_1, 300, &im_bk, 0, 0, SRCCOPY);
		putimagePng(dx_1, 0, &im_hill);
		putimagePng(dx_1 - WIDTH, 0, &im_hill);

		putimage(0, 300, dx_2, 480, &im_bk, WIDTH - dx_2, 300, SRCCOPY);
		putimage(dx_2, 300, WIDTH - dx_2, 480, &im_bk, 0, 300, SRCCOPY);
		putimagePng(dx_2, 300, &im_house);
		putimagePng(dx_2 - WIDTH, 300, &im_house);

		putimagePng(WIDTH * 2 / 3, 260, &img[i]);
		i++;
		if (i > 7)
			i = 0;

		Sleep(60);
		FlushBatchDraw();

		dx_1 = dx_1 + speed_1;
		if (dx_1 >= WIDTH)
			dx_1 = speed_1;

		dx_2 = dx_2 + speed_2;
		if (dx_2 >= WIDTH)
			dx_2 = speed_2;

	}

	_getch();
	return 0;
}

