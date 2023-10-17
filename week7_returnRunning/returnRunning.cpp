
#include <stdio.h>
#include <graphics.h>
#include "conio.h"
#include "EasyXPng.h"

#define WIDTH  640
#define HEIGHT 480

int speed, x;
bool direction; //true->right, false->left

int main()
{
	IMAGE im_bk, im_dra;
	initgraph(WIDTH, HEIGHT);

	int i = 0;
	TCHAR filename[20];
	IMAGE img[8];

	for (i = 0; i < 8; i++)
	{
		_stprintf_s(filename, _T("material\\g%d.png"), i);
		loadimage(&img[i], filename);
	}

	loadimage(&im_bk, _T("material\\bg.png"));

	i = 0;
	speed = 5, x = img[0].getwidth();
	direction = true;
	BeginBatchDraw();
	while (1)
	{
		putimage(0, 0, &im_bk);

		// right
		if (direction == true)
		{
			if (i < 4 || i > 7)
				i = 4;

			putimagePng(x, HEIGHT / 2, &img[i]);
			x += speed;
		}

		// left
		else if (direction == false)
		{
			if (i > 3) 
				i = 0;

			putimagePng(x, HEIGHT / 2, &img[i]);
			x -= speed;
		}

		if (x < 0 || x > WIDTH - img[0].getwidth())
			direction = !direction;

		Sleep(50);

		i++;
		FlushBatchDraw();
	}

	_getch();
	return 0;
}