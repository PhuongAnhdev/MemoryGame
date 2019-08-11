#pragma once

#include <iostream>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

const int TTstep[9]= {0,3,5,6,8,12,16,25,37};

struct point
{
    int x,y;
};

bool Play();
