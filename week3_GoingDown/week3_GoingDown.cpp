#include <graphics.h>  
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct Board
{
    double x, y;
    int special;
};

int main()
{
    double w = 600, h = 600;
    double bw = 100, bh = 10;
    double speed = 2;
    double x, y, r = 10;
    double vx = 5, vy = 0, g = 0.2;
    int i = 0;
    
    TCHAR count_char[100];
    int life = 5;
    bool newball = false;
    int slow = 0;

    srand((unsigned int)time(NULL));

    Board board[6];

    initgraph(w, h);
    settextcolor(RGB(255, 0, 0));
    settextstyle(20, 0, _T("ËÎÌו"));
    for (i = 0; i < 6; i++)
    {
        board[i].x = rand() % 500;
        board[i].y = 600;
        board[i].special = rand() % 20;
    }
    x = board[0].x + bw / 2, y = board[0].y - r;

    BeginBatchDraw();
    while (1)
    {
        if (slow > 0)
        {
            --slow;
            speed = 1;
        }
        else
        {
            speed = 2;
            slow = 0;
        }

        if (life == 0) 
        {
            break;
        }

        if ((y <= 25 + r || y >= 650) && newball == false)
        {
            --life;
            newball = true;
        }

        y += vy;

        for (i = 0; i < 20; i++)
        {
            line(30 * i, 0, 30 * i + 15, 30);
            line(30 * i + 15, 30, 30 * (i + 1), 0);
        }
        bool collosion = false;
        for (i = 0; i < 6; i++)
        {
            if (i == 0 || board[i].y - board[i - 1].y >= 95 || board[i].y <= 105)
            {
                // new ball after death
                if (newball == true && board[i].y >= 500)
                {
                    newball = false;
                    x = board[i].x + bw / 2, y = board[i].y - r;
                }
                if (collosion == false && x + r >= board[i].x && x - r <= board[i].x + bw && y + r >= board[i].y && y - r <= board[i].y + bh)
                {
                    if (x >= board[i].x - r && x <= board[i].x + bw + r)
                    {
                        vy = 0;
                        if (y < board[i].y + bh - speed)
                        {
                            collosion = true;
                            // tp to the bottom block
                            if (board[i].special == 5 || board[i].special == 6 || board[i].special == 7)
                            {
                                for (int j = 0; j < 6; j++)
                                {
                                    if (board[j].y >= 500)
                                    {
                                        x = board[j].x + bw / 2, y = board[j].y - r;
                                    }
                                }
                                board[i].special = 0;
                            }
                            else
                            {
                                y = board[i].y - r - speed;
                            }
                        }
                        else
                        {
                            y = board[i].y + bh + r - speed;
                        }
                    }
                    else
                    {
                        if (x < board[i].x)
                        {
                            x = board[i].x - r;
                        }
                        else
                        {
                            x = board[i].x + bw + r;
                        }
                    }
                }
                board[i].y = board[i].y - speed;
                // 5-7=tp block
                if (board[i].special == 5 || board[i].special == 6 || board[i].special == 7)
                    rectangle(board[i].x, board[i].y, board[i].x + bw, board[i].y + bh);
                else
                    fillrectangle(board[i].x, board[i].y, board[i].x + bw, board[i].y + bh);
            }
            // 8=add a life
            if (board[i].special == 8) 
            {
                setfillcolor(RGB(0, 255, 0));
                fillrectangle(board[i].x + bw / 2 - 5, board[i].y - 6, board[i].x + bw / 2 + 5, board[i].y - 4);
                fillrectangle(board[i].x + bw / 2 - 1, board[i].y - 10, board[i].x + bw / 2 + 1, board[i].y);
                setfillcolor(RGB(255, 255, 255));
                if (x + r >= board[i].x + bw / 2 - 5 && x - r <= board[i].x + bw / 2 + 5 && y + r >= board[i].y - 10 && y - r <= board[i].y)
                {
                    ++life;
                    board[i].special = 0;
                }
            }
            // 9=slow the speed
            if (board[i].special == 9 || board[i].special == 10)
            {
                setfillcolor(RGB(0, 0, 255));
                fillrectangle(board[i].x + bw / 2 - 5, board[i].y - 10, board[i].x + bw / 2 + 5, board[i].y);
                setfillcolor(RGB(255, 255, 255));
                if (x + r >= board[i].x + bw / 2 - 5 && x - r <= board[i].x + bw / 2 + 5 && y + r >= board[i].y - 10 && y - r <= board[i].y)
                {
                    slow += 150;
                    board[i].special = 0;
                }
            }

            if (board[i].y < 0)
            {
                board[i].y = h - bh;
                board[i].x = rand() % 500;
                board[i].special = rand() % 20;
            }
        }


        if (collosion == true) 
        {
            vy = 0;
        }
        else
        {
            vy += g;
        }

        if (_kbhit())
        {
            char input = _getch();
            if (input == ' ' && collosion == true)
            {
                vy = -9;
            }
            if (GetAsyncKeyState(VK_LEFT))
            {
                x -= vx;
                if (x <= 0 + r)
                    x = 0 + r;
            }
            if (GetAsyncKeyState(VK_RIGHT))
            {
                x += vx;
                if (x >= w - r)
                    x = w - r;
            }
        }
        fillcircle(x, y, r);
        swprintf_s(count_char, _T("%d"), life);
        outtextxy(10, 30, _T("LIFE: "));
        outtextxy(70, 30, count_char);
        FlushBatchDraw();
        Sleep(15);
        cleardevice();
    }
    settextstyle(50, 0, _T("ËÎÌו"));
    outtextxy(200, 250, _T("GAME OVER"));
    FlushBatchDraw();
    cleardevice();
    while (true);
    _getch();
    closegraph();
    return 0;
}