#pragma once

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

SDL_Texture* loadTexture(string path, SDL_Renderer* Renderer)
{
    SDL_Texture* newTexture= NULL;
    SDL_Surface* loadedSurface= IMG_Load(path.c_str());
    if (loadedSurface==NULL) {cout<<"Error load image\n"; exit(1);}
    newTexture= SDL_CreateTextureFromSurface(Renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

SDL_Texture* loadText(TTF_Font *font, string text, SDL_Color color, SDL_Renderer* Renderer)
{
    SDL_Texture* newTexture= NULL;
    SDL_Surface* textSurface= TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface==NULL) {cout<<"Error font\n"; exit(1);}
    newTexture= SDL_CreateTextureFromSurface(Renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return newTexture;
}

string numToStr(int k)
{
    string s="";
    if (k==0) return "0";
    char x;
    int i=0;
    while (k>0){
        x= (k%10)+48;
        s= x+s;
        i++;
        if (i==3) {
            if (k>9) s=','+s;
            i=0;
        }
        k/=10;
    }
    return s;
}