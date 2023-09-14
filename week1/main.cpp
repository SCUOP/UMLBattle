#include <graphics.h>
#include <stdio.h>
#include <conio.h>

// max color value
#define max_color 255
// define length
#define width 1600
// define width
#define length 1000
// define frequency of color change
#define color_frame 8
// timer
#define timer 50

int main() {

	// init the variable
	// left center of circle
	int ldot = width / 4;
	// right center of circle
	int rdot = width * 3 / 4;

	// init the background
	initgraph(width, length);
	setbkcolor(WHITE);
	cleardevice();

	// draw
	for (int step = 1; step <= max_color; step += color_frame) {
		// draw the left circle
		setlinecolor(RGB(step, 0, 0));
		circle(ldot, length / 2, step);
		// draw the right circle
		setlinecolor(RGB(0, max_color - step, 0));
		circle(rdot, length / 2, step);
		// delay
		Sleep(timer);
	}

	// prevent window from closing
	_getch();

	return 0;
}