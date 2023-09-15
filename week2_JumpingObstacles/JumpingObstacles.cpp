#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <conio.h>

int main()
{
    srand((int)time(NULL));

    // ball's initial position
    int x = 200, y = 380, r = 20;
    // the screen's initial position
    int w = 600, h = 400;
    // vy and g for ball
    int vy = 0, g = 1;
    // initial position of the rectangle
    int left = 570, top = 350 - rand() % 80, right = 600, bottom = 400;
    // vx for rectangle
    int vx = rand() % 6 + 5;
    // create the graph
    initgraph(w,h);
    // counter
    int count = 0;
    TCHAR count_char[100];
    bool counter = true;
    settextstyle(20, 0, _T("ËÎÌו"));

    BeginBatchDraw();

    while (true) {

        // refresh location of rectangle
        left -= vx;
        right -= vx;

        // rectangle has cross the border
        if (right <= 0)
        {
            left = 570, top = 350 - rand() % 80, right = 600, bottom = 400;
            vx = rand() % 6 + 5;
            counter = true;
        }
        
        // detect the keyboard input
        if (_kbhit()) {
            char input = _getch();
            if (input == ' ' && y == 380) {
                vy = -20;
            }
        }
        
        // refresh location of ball
        vy = vy + g;
        y = y + vy;
        
        // make sure the ball doesn't sink
        if (y >= h-r) {
            vy = 0;
            y = h - r;
        }

        // collision checking
        if (left <= x + r && right >= x - r && top <= y + r) {
            count = 0;
            left += vx;
            if (left >= x + r) {
                left = x + r;
                right = left + 30;
            }
            else {
                left -= vx;
                vy = 0;
                y = top - r;
            }
        }

        // count score
        if (right < x - r && counter) {
            count++;
            counter = false;
        }

        // draw the graph
        cleardevice();
        swprintf_s(count_char, _T("%d"), count);
        outtextxy(50, 30, _T("SCORE: "));
        outtextxy(120, 31, count_char);
        fillcircle(x, y, r);
        fillrectangle(left, top, right, bottom);

        FlushBatchDraw();

        Sleep(10);
    }
    EndBatchDraw();
    closegraph();
    _getch();
    return 0;
}