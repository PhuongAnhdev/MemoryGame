#include <iostream>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Card.h"

using namespace std;


Card::Card(){
	gotten = false;
	upface = false;
}

Card::Card(int number){
	gotten = false;
	upface = false;
	this->number = number;
}

void Card::do_gotten(){
	gotten = true;
}

void Card::un_gotten(){
	gotten = false;
}

void Card::do_upface(){
	upface = true;
}

void Card::un_upface(){
	upface = false;
}

bool Card::is_gotten(){
	return gotten;
}

bool Card::is_upface(){
	return upface;
}

int Card::get_number(){
	return number;
}

void Card::set_number(int number){
	this->number = number;
}
