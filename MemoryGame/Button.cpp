#include <iostream>

#include "Button.h"

using namespace std;

Button::Button(){
}

Button::Button(int xmin, int xmax, int ymin, int ymax, int ev){
	this->xmax = xmax;
	this->xmin = xmin;
	this->ymax = ymax;
	this->ymin = ymin;
	this->my_event = ev;
}

bool Button::check_press_on(int x, int y){
	return (x >= xmin && x <= xmax && y >= ymin && y <=ymax);
}

int Button::on_click_return(){
	return my_event;
}

void Button::set(int xmin, int xmax, int ymin, int ymax, int ev){
	this->xmax = xmax;
	this->xmin = xmin;
	this->ymax = ymax;
	this->ymin = ymin;
	this->my_event = ev;
}
