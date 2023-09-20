#include <graphics.h>  
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct Board
{
	int x, y;
};

int main()
{
	int w = 600, h = 600;
	int bw = 100, bh = 10;
	int speed = 5;
	int x, y = 590;
	int i = 0;

	srand((unsigned int)time(NULL));

	Board board[6];

	initgraph(w, h);

	for (i = 0; i < 6; i++)
	{
		board[i].x = rand() % 500;
		board[i].y = 590;
	}

	BeginBatchDraw();
	while (1)
	{
		for (i = 0; i < 20; i++)
		{
			line(30 * i, 0, 30 * i + 15, 30);
			line(30 * i + 15, 30, 30 * (i + 1), 0);
		}
		for (i = 0; i < 6; i++)
		{
			if (i == 0 || board[i].y - board[i - 1].y > 100 || board[i].y <= 100)
			{
				fillrectangle(board[i].x, board[i].y, board[i].x + bw, board[i].y + bh);
				board[i].y = board[i].y - speed;
			}
			if (board[i].y < 0)
			{
				board[i].y = h - bh;
				board[i].x = rand() % 500;
			}
		}
		FlushBatchDraw();
		Sleep(100);
		cleardevice();
	}
	_getch();
	closegraph();
	return 0;
}