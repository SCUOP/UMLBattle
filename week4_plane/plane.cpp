
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <list>

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

//	Detection of keyboard pressing
void IsPressKey()
{
	if (_kbhit())			//	If a key is pressed
	{
		char key;
		key = _getch();		//	Obtain key info

		if (GetAsyncKeyState(VK_UP))     //Up
			myPlane.y -= 8;
		if (GetAsyncKeyState(VK_DOWN))   //Down
			myPlane.y += 8;
		if (GetAsyncKeyState(VK_LEFT))   //Left
			myPlane.x -= 6;
		if (GetAsyncKeyState(VK_RIGHT))  //Right
			myPlane.x += 6;
	}
}


//	Detection of Collision
void Shoot() {

	list<Node>::iterator pDj = enemyPlaneList.begin();
	list<Node>::iterator pZd = bulletList.begin();

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
}

int main()
{
	// Random seed
	srand((unsigned int)time(NULL));
	//	Create the screen
	initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);

	DWORD t1, t2;			//	Speed of enemy planes
	DWORD tt1, tt2;			//	Speed of the bullets

	t1 = GetTickCount();			//	Starting time of the plane
	tt1 = GetTickCount();			//	Starting time of the bullet

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

		BeginBatchDraw();		  //Starting batchdraw
		cleardevice();

		//show our plane at (x=260, y=740)
		rectangle(myPlane.x, myPlane.y, myPlane.x + 32, myPlane.y + 18);

		//show our bullet
		for (auto& p : bulletList) {
			//Print the bullet
			circle(p.x, p.y, 5);
			//	moving upwards
			p.y--;
		}

		// Show the enemy plane
		for (auto& p : enemyPlaneList) {
			//Print the plane
			roundrect(p.x, p.y, p.x + 30, p.y + 20, 5, 5);
			// Moving downwards
			p.y++;
		}

		Sleep(5);

		Shoot(); // detection of collision

		EndBatchDraw(); // end batchdraw

		IsPressKey(); // detection of keyboard signal

	}

	getchar();

	return 0;
}