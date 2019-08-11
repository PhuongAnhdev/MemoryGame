#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


using namespace std;

SDL_Texture* loadTexture(string path, SDL_Renderer* Renderer);
SDL_Texture* loadText(TTF_Font *font, string text, SDL_Color color,  SDL_Renderer* Renderer);
string numToStr(int k);
