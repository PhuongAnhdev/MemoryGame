#pragma once

#include <iostream>

using namespace std;

static const int EVENT_PLAY = 0;
static const int EVENT_SCORE = 1;
static const int EVENT_TUTORIAL = 2;
static const int EVENT_QUIT = 3;
static const int EVENT_SOUND = 4;
static const int NO_CLICK = 5;

class Button
{
private:
	int xmax;
	int ymax;
	int xmin;
	int ymin;
	int my_event;
public:
	Button();
	Button(int xmin, int xmax, int ymin, int ymax, int ev);
	void set(int xmin, int xmax, int ymin, int ymax, int ev);
	bool check_press_on(int x, int y);
	int on_click_return();
};
