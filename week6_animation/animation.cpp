#include <graphics.h>  
#include <conio.h>
#include <time.h>
#include "EasyXPng.h"  

#pragma comment(lib,"Winmm.lib")

#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4996)

#define  WIDTH  640 
#define  HEIGHT 480 

int main()
{
	IMAGE bkg;
	loadimage(&bkg, _T("material\\bkg.png"));
	initgraph(WIDTH, HEIGHT);

	int i = 0;
	TCHAR filename[20];
	IMAGE img[8];
	for (i = 0; i < 8; i++)
	{
		_stprintf(filename, _T("material\\man%d.png"), i);
		loadimage(&img[i], filename);
	}

	mciSendString(_T("open material\\bgm.mp3 alias bkmusic"), NULL, 0, NULL);

	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);


	i = 0;
	while (1)
	{
		putimage(0, 0, &bkg);
		putimagePng(0, 0, &img[i]);
		Sleep(200);
		i += 1;
		if (i == 8) i = 0;
	}
	_getch();
	return 0;
}