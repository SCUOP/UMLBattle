
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <list>
#include "EasyXPng.h"

using namespace std;
//#pragma comment(lib,"winmm.lib")
//#pragma warning(disable:4996);

//Define area
#define WIN_WIDTH	600
#define	WIN_HEIGHT	800


//Definition of coordinate of both planes and bullets
class Node {
public:
	Node(int xx, int yy) {
		x = xx;
		y = yy;
	}
	int x;
	int y;
};

//Create List
/*
1¡¢The enemy planes come from the top to the bottom
2, The bullets fired by our plane come from bottom to the top
*/

Node myPlane(260, 740);
list<Node> enemyPlaneList;
list<Node> bulletList;
list<Node> enemyBulletList;
int blood = 10;
int dy = 0, bk_speed = 1;

//	Detection of keyboard pressing
void IsPressKey()
{
	if (_kbhit())			//	If a key is pressed
	{
		char key;
		key = _getch();		//	Obtain key info

		if (GetAsyncKeyState(VK_UP))     //Up
		{
			myPlane.y -= 3;
			if (myPlane.y < 20)
				myPlane.y = 20;
		}
		if (GetAsyncKeyState(VK_DOWN))   //Down
		{
			myPlane.y += 3;
			if (myPlane.y > WIN_HEIGHT - 18)
				myPlane.y = WIN_HEIGHT - 18;
		}
		if (GetAsyncKeyState(VK_LEFT))   //Left
		{
			myPlane.x -= 4;
			if (myPlane.x < 0)
				myPlane.x = 0;
		}
		if (GetAsyncKeyState(VK_RIGHT))  //Right
		{
			myPlane.x += 4;
			if (myPlane.x > WIN_WIDTH - 32)
				myPlane.x = WIN_WIDTH - 32;
		}
	}
}


//	Detection of Collision
void Shoot() {

	list<Node>::iterator pDj = enemyPlaneList.begin();
	list<Node>::iterator pZd = bulletList.begin();
	list<Node>::iterator pEzd = enemyBulletList.begin();

	//	Iterate all planes 
	bool flag = false;
	while (pDj != enemyPlaneList.end()) {
		pZd = bulletList.begin();
		//	Iterate all bullets
		while (pZd != bulletList.end()) {
			//	Detection of Collision
			if ((*pZd).x >= ((*pDj).x - 10) && (*pZd).x <= ((*pDj).x + 50) &&
				(*pZd).y >= ((*pDj).y - 15) && (*pZd).y <= ((*pDj).y + 30)
				) {
				//	If collide, them eliminate both the plane and the bullet
				enemyPlaneList.erase(pDj);
				bulletList.erase(pZd);
				flag = true;
				break;
			}
			else {
				++pZd;
			}
		}
		if (flag) {
			break;
		}
		++pDj;
	}

	while (pEzd != enemyBulletList.end())
	{
		list<Node>::iterator Ezd = pEzd;
		++pEzd;
		if (Ezd->x + 5 >= (myPlane.x) && Ezd->x - 5 <= (myPlane.x + 32) && Ezd->y + 5 >= (myPlane.y) && Ezd->y - 5 <= (myPlane.y + 18))
		{
			blood -= 1;
			enemyBulletList.erase(Ezd);
		}
	}

	pDj = enemyPlaneList.begin();
	while (pDj != enemyPlaneList.end())
	{
		if ((pDj->x + 30) >= (myPlane.x) && (pDj->x) <= (myPlane.x + 32) && (pDj->y + 30) >= (myPlane.y) && (pDj->y - 15) <= (myPlane.y + 18))
		{
			blood -= 1;
			enemyPlaneList.erase(pDj);
			break;
		}
		pDj++;
	}
}

int main()
{
	IMAGE im_bk, im_player, im_enemy, im_bullet, im_enemy_bullet;
	loadimage(&im_bk, _T("material\\bk.png"));
	loadimage(&im_player, _T("material\\player.png"));
	loadimage(&im_enemy, _T("material\\enemy.png"));
	loadimage(&im_bullet, _T("material\\bullet.png"));
	loadimage(&im_enemy_bullet, _T("material\\enemy_bullet.png"));

	// Random seed
	srand((unsigned int)time(NULL));
	//	Create the screen
	initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);
	setfillcolor(RGB(255, 127, 127));

	DWORD t1, t2;			//	Speed of enemy planes
	DWORD tt1, tt2;			//	Speed of the bullets
	DWORD et1, et2;			//  Speed of the enemy's bullets

	t1 = GetTickCount();			//	Starting time of the plane
	tt1 = GetTickCount();			//	Starting time of the bullet
	et1 = GetTickCount();			//  Starting time of the enemy's bullet

	while (1)
	{
		//	Add an enemy plane every 1000ms
		t2 = GetTickCount();
		if (t2 - t1 >= 1000) {
			enemyPlaneList.push_back(Node(rand() % (WIN_WIDTH - 50), 0));
			t1 = t2;
		}

		//	Add a bullet every 500ms
		tt2 = GetTickCount();
		if (tt2 - tt1 >= 500) {
			bulletList.push_back(Node(myPlane.x + 20, myPlane.y - 20));
			tt1 = tt2;
		}

		// Add a ememy's bullet every 500ms with 1/2 probability
		et2 = GetTickCount();
		if (et2 - et1 >= 500)
		{
			list<Node>::iterator pDj = enemyPlaneList.begin();
			while (pDj != enemyPlaneList.end())
			{
				if (rand() % 2 == 0)
					enemyBulletList.push_back(Node(pDj->x + 20, pDj->y + 18 + 10));
				++pDj;
			}
			et1 = et2;
		}

		BeginBatchDraw();		  //Starting batchdraw
		cleardevice();

		putimagePng(0, dy, &im_bk);
		putimage(0, dy - WIN_HEIGHT, &im_bk);
		dy += bk_speed;
		if (dy >= WIN_HEIGHT)
			dy = bk_speed;

		//show our plane at (x=260, y=740)
		/*rectangle(myPlane.x, myPlane.y, myPlane.x + 32, myPlane.y + 18);*/
		putimagePng(myPlane.x, myPlane.y, &im_player);

		//show our bullet
		for (auto& p : bulletList) {
			//Print the bullet
			/*circle(p.x, p.y, 5);*/
			putimagePng(p.x-5, p.y-5, &im_bullet);
			//	moving upwards
			p.y -= 3;
		}

		// Show the enemy plane
		for (auto& p : enemyPlaneList) {
			//Print the plane
			/*roundrect(p.x, p.y, p.x + 30, p.y + 20, 5, 5);*/
			putimagePng(p.x, p.y, &im_enemy);
			// Moving downwards
			p.y += 2;
		}

		// Show the enemy's bullet
		setlinecolor(RGB(255, 127, 127));
		for (auto& p : enemyBulletList)
		{
			//Print the bullet
			/*circle(p.x, p.y, 5);*/
			putimagePng(p.x - 5, p.y - 5, &im_enemy_bullet);
			//	moving upwards
			p.y += 3;
		}
		setlinecolor(WHITE);

		// Show the blood
		rectangle(0, 0, 200, 20);
		fillrectangle(0, 0, blood * 20, 20);

		// game over
		if (blood == 0)
			break;

		Sleep(5);

		Shoot(); // detection of collision

		EndBatchDraw(); // end batchdraw

		IsPressKey(); // detection of keyboard signal

	}
	settextstyle(50, 0, _T("ËÎÌå"));
	outtextxy(180, 350, _T("GAME OVER"));
	FlushBatchDraw();
	cleardevice();
	while (true);
	getchar();

	return 0;
}