#include <graphics.h>
#include <conio.h>
#include <stdio.h>

int main()
{
	int r = 10;
	int x = 300, y = 750;
	int vx = 8, vy = -10;
	int i, j;
	int w = 90, h = 40;
	int xs, ys;
	int gap = 5;
	int bx = 250, len = 100, th = 15;
	bool s = false;
	bool f = false;

	bool flag[5][6];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			flag[i][j] = false;
		}
	}

	int left, right, top, bottom;

	initgraph(600, 600);

	setbkcolor(RGB(200, 200, 200));
	setlinecolor(RGB(0, 128, 128));
	setfillcolor(RGB(0, 128, 128));
	cleardevice();

	BeginBatchDraw();
	while (1)
	{
		if (s == false) {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 6; j++) {
					flag[i][j] = false;
				}
			}
		}

		setfillcolor(RGB(0, 128, 128));
		bool collision = false;
		for (i = 0; i < 6; i++) // create a 6 * 5 matrix
			for (j = 0; j < 5; j++)
			{
				if (flag[j][i] == false) {
					xs = i * w;
					ys = j * h;
					left = gap * (i + 1) + xs;
					top = gap * (j + 1) + ys;
					right = gap * (i + 1) + xs + w;
					bottom = gap * (j + 1) + ys + h;
					if (collision == false && (left <= x + r && right >= x - r && top <= y + r && bottom >= y - r)) {
						flag[j][i] = true;
						collision = true;
						if (x >= left && x <= right)
							vy = -vy;
						else if (y >= top && y <= bottom)
							vx = -vx;
						else {
							vy = -vy;
							vx = -vx;
						}
					}
					else {
						fillrectangle(gap * (i + 1) + xs, gap * (j + 1) + ys, gap * (i + 1) + xs + w, gap * (j + 1) + ys + h);
					}
				}
			}

		setfillcolor(RGB(0, 0, 128));
		fillrectangle(bx, 560, bx + len, 560 + th); // draw the board

		if (_kbhit())
		{
			char input = _getch();
			if (input == ' ')
			{
				s = true; // game start
				f = true;
			}
			if (GetAsyncKeyState(VK_LEFT))
			{
				bx = bx - 8;
				if (bx <= 0)
					bx = 0;
			}
			if (GetAsyncKeyState(VK_RIGHT))
			{
				bx = bx + 8;
				if (bx >= 600)
					bx = 600;
			}
		}
		if (!s)  //  before game has been started
		{
			setfillcolor(RGB(128, 0, 0));
			fillcircle(bx + len / 2, 560 - r, r);
		}
		else  //  after game has been started
		{
			if ((((y > 550) && (y < 600)) && (x + r >= bx) && (x - r <= bx + len)) || (y < 0))
				vy = -vy;
			if ((x > 600 - r) || (x < r))
				vx = -vx;

			if (f) // initial position of the ball
			{
				x = bx + len / 2;
				y = 560 - r;
				f = !f;
			}
			else  // the position of the ball during the game
			{
				x = x + vx;
				y = y + vy;
				if (y >= 600)
				{
					settextcolor(RGB(0, 0, 255));
					settextstyle(60, 0, _T("Arial"));
					outtextxy(270, 400, _T("Lose"));
					//  break;
				}
			}
			setfillcolor(RGB(128, 0, 0));
			fillcircle(x, y, r);
		}

		FlushBatchDraw();
		Sleep(30);
		cleardevice();
	}
	closegraph();
	return 0;
}