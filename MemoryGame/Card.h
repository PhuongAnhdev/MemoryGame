#pragma once

#include <iostream>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

class Card
{
private:
	int number;
	bool upface, gotten;
public:
	Card();
	Card(int number);
	bool is_upface();
	bool is_gotten();
	void do_upface();
	void un_upface();
	void do_gotten();
	void un_gotten();
	void set_number(int number);
	int get_number();
};
