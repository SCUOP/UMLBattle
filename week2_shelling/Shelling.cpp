#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

int main() {

	srand((int)time(NULL));

	// initialize variables
	int x, y;
	int vx = 0, vy = 0;
	int g = 1;
	int r = 15;
	int p, l, vp;

	// check if firing
	bool f = false;

	// initgraph
	initgraph(600, 800);
	// counter
	int count = 0;
	TCHAR count_char[100];
	bool counter = true;
	settextstyle(20, 0, _T("ËÎÌו"));

	BeginBatchDraw();
	setbkcolor(RGB(50, 50, 50));
	cleardevice();
	p = 50;
	l = 50 + rand() % 100;

	x = 0;
	y = 600;
	vp = rand() % 10 + 5;
	while (true) {
		fillrectangle(580, p, 600, p + l);
		
		p += vp;

		// rebound
		if (p >= 800 || p <= -l) {
			l = 50 + rand() % 100;
			vp = rand() % 10 + 2;
			if (p >= 800)
				vp = -vp;
		}
		
		// firing
		if (_kbhit()) {
			char input = _getch();
			if (input == ' ' && f == false) {
				f = true;
				vx = 15;
				vy = -30;
			}
		}

		// control the position of ball
		if (f) {
			vy += g;
			x += vx;
			y += vy;
		}
		else {
			x = 0;
			y = 600;
		}

		// the ball is out of bounds
		if (x >= 650) {
			x = 0;
			y = 600;
			f = false;
		}

		// collision checking
		if (580 <= x + r && 600 >= x - r && p <= y + r && p + l >= y-r) {
			count += 1;
			x = 0;
			y = 600;
			f = false;
		}
		fillcircle(x, y, r);
		swprintf_s(count_char, _T("%d"), count);
		outtextxy(50, 30, _T("SCORE: "));
		outtextxy(120, 31, count_char);

		FlushBatchDraw();
		Sleep(16);
		cleardevice();
	}

	EndBatchDraw();
	_getch();
	return 0;
}