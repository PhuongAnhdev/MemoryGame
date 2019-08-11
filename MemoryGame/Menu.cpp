#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Menu.h"
#include "Utils.h"
#include "Button.h"

using namespace std;

const int WIDTH= 1200;
const int HEIGHT= 680;
const string TITLE= "Memory Game";

enum
{
    MEASY=0, MMEDIUM, MHARD
};

enum selection
{
    PLAY=0,
    SCORE, HELP, EXIT
};

SDL_Window* MWindow;
SDL_Surface* MScreen;
SDL_Renderer * MRenderer;
SDL_Texture *Logo, *BG, *BG2, *back_button, *quit, *reg, *uet, *prePlay;
SDL_Texture *menu[4][2], *tutor[2], *arrow[2], *Y[2], *N[2], *hscore[3], *easy[2], *med[2], *hard[2], *play[2];
SDL_Texture *Mspeaker[2];
bool status[6]={false}, Msound=1;
string hc_name[5];
int hc_score[5];
SDL_Texture *hnamew[5], *hscorew[5];
TTF_Font* font;
Mix_Music* menu_beat;
Mix_Chunk* Mclick;
Button menuButton[NO_CLICK];


void MloadImage()
{
    reg= loadTexture("menu/reg.PNG", MRenderer);
    SDL_SetTextureBlendMode(reg, SDL_BLENDMODE_BLEND);
    uet= loadTexture("menu/UET.PNG", MRenderer);
    SDL_SetTextureBlendMode(uet, SDL_BLENDMODE_BLEND);

    Logo= loadTexture("menu/Game_Logo.PNG", MRenderer);
    BG= loadTexture("menu/BG.PNG", MRenderer);
    back_button= loadTexture("menu/back.PNG", MRenderer);
    quit= loadTexture("menu/quit.PNG", MRenderer);
    prePlay= loadTexture("menu/game_mode.PNG", MRenderer);

    menu[0][0]= loadTexture("menu/PLAY.PNG", MRenderer);
    menu[0][1]= loadTexture("menu/PLAY_s.PNG", MRenderer);
    menu[1][0]= loadTexture("menu/SCORE.PNG", MRenderer);
    menu[1][1]= loadTexture("menu/SCORE_s.PNG", MRenderer);
    menu[2][0]= loadTexture("menu/HELP.PNG", MRenderer);
    menu[2][1]= loadTexture("menu/HELP_s.PNG", MRenderer);
    menu[3][0]= loadTexture("menu/EXIT.PNG", MRenderer);
    menu[3][1]= loadTexture("menu/EXIT_s.PNG", MRenderer);
    Mspeaker[0]= loadTexture("menu/mute.PNG", MRenderer);
    Mspeaker[1]= loadTexture("menu/speaker.PNG", MRenderer);
    tutor[0] = loadTexture("menu/TUTOR1.PNG", MRenderer);
    tutor[1] = loadTexture("menu/TUTOR2.PNG", MRenderer);
    arrow[0] = loadTexture("menu/arrow1.PNG", MRenderer);
    arrow[1] = loadTexture("menu/arrow2.PNG", MRenderer);
    hscore[0]= loadTexture("menu/highscore.PNG", MRenderer);
    hscore[1]= loadTexture("menu/highscore.PNG", MRenderer);
    hscore[2]= loadTexture("menu/highscore.PNG", MRenderer);
    Y[0]= loadTexture("menu/YES.PNG", MRenderer);
    Y[1]= loadTexture("menu/YES_s.PNG", MRenderer);
    N[0]= loadTexture("menu/NO.PNG", MRenderer);
    N[1]= loadTexture("menu/NO_s.PNG", MRenderer);
    easy[0]= loadTexture("menu/EASY.PNG", MRenderer);
    easy[1]= loadTexture("menu/EASY_s.PNG", MRenderer);
    med[0]= loadTexture("menu/MEDIUM.PNG", MRenderer);
    med[1]= loadTexture("menu/MEDIUM_s.PNG", MRenderer);
    hard[0]= loadTexture("menu/HARD.PNG", MRenderer);
    hard[1]= loadTexture("menu/HARD_s.PNG", MRenderer);
    play[0]= loadTexture("menu/playnow.PNG", MRenderer);
    play[1]= loadTexture("menu/playnow_s.PNG", MRenderer);
}

void MloadTrack()
{
    menu_beat= Mix_LoadMUS("menu_soundtrack/menu_beat.mp3");
    Mclick= Mix_LoadWAV("menu_soundtrack/click.mp3");
}

void MinitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING)==-1) exit(1);
    MWindow= SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    MScreen= SDL_GetWindowSurface(MWindow);

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    MRenderer= SDL_CreateRenderer(MWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(MRenderer, WIDTH, HEIGHT);
    SDL_SetRenderDrawColor(MRenderer,0,0,0,0);

    font= TTF_OpenFont("menu/BERNHC.ttf", 40);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)<0) exit(1);

    MloadTrack();
    MloadImage();
}


int Position(int x_, int y_)
{
	for (int i = 0 ; i<NO_CLICK; i++){
		if (menuButton[i].check_press_on(x_, y_)){
			return menuButton[i].on_click_return();
		}
	}
	return NO_CLICK;
}

void MquitSDL()
{
    Mix_FreeMusic(menu_beat);
    Mix_FreeChunk(Mclick);
    SDL_FreeSurface(MScreen);
    SDL_DestroyRenderer(MRenderer);
    SDL_DestroyWindow(MWindow);
    TTF_Quit();
    IMG_Quit();
    //Mix_Quit();
    //SDL_Quit();
}

void Mapply_surface(int x, int y, int w, int h, SDL_Texture* source)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    offset.w = w;
    offset.h = h;
    SDL_RenderCopy(MRenderer, source, NULL, &offset);
}

void apply_BG(int x, SDL_Texture* source)
{
    SDL_Rect off1 = {x,0,1360-x,680};
    SDL_Rect off2 = {0,0,off1.w,680};
    SDL_RenderCopy(MRenderer, source, &off1, &off2);
    if (x+1200<=1360) return;
	off1.x = 0;
	off1.y = 0;
	off1.w = x;
	off1.h = 680;
	
	off2.x = 1360-x;
	off2.y = 0;
	off2.w = off1.w;
	off2.h = 680;
    SDL_RenderCopy(MRenderer, source, &off1, &off2);
}

void MTransound(int* k,SDL_Texture* T1,SDL_Texture* T2)
{
    Mix_PlayChannel( -1, Mclick, 0 );
    for (int i=0; i<=25; i++) {
        apply_BG(*k,BG);
        Mapply_surface(228,25,743,198,Logo);
        for (int j=0; j<4; j++) Mapply_surface(486, 250+j*105, 228, 100, menu[j][status[j]]);
        Mapply_surface(1075+i*2,555,100-i*4,100,T1);
        SDL_RenderPresent(MRenderer);
        SDL_Delay(3);
        (*k)++;
    }

    if(Mix_PausedMusic()) Mix_ResumeMusic();
        else Mix_PauseMusic();


    for (int i=25; i>=0; i--) {
        apply_BG(*k,BG);
        Mapply_surface(228,25,743,198,Logo);
        for (int j=0; j<4; j++) Mapply_surface(486, 250+j*105, 228, 100, menu[j][status[j]]);
        Mapply_surface(1075+i*2,555,100-i*4,100,T2);
        SDL_RenderPresent(MRenderer);
        SDL_Delay(3);
        (*k)++;
    }

    Msound=!Msound;
}

void readScore(int diffi)
{
    ifstream fi;
	fi.open("high_score.txt",ios::in);

    for (int i=0; i<5; i++){
        getline(fi,hc_name[i]);
        fi>>hc_score[i];
    }
	SDL_Color color;
	color.r = 250;
	color.g = 240;
	color.b = 0;
	
    hnamew[0]= loadText(font, hc_name[0], color, MRenderer);
    hscorew[0]= loadText(font, numToStr(hc_score[0]), color, MRenderer);
	
	color.r = 126;
	color.g = 102;
	color.b = 102;
    hnamew[1]= loadText(font, hc_name[1], color, MRenderer);
    hscorew[1]= loadText(font, numToStr(hc_score[1]), color, MRenderer);
	
	color.r = 139;
	color.g = 57;
	color.b = 7;
	hnamew[2]= loadText(font, hc_name[2], color, MRenderer);
    hscorew[2]= loadText(font, numToStr(hc_score[2]), color, MRenderer);

	color.r = 22;
	color.g = 168;
	color.b = 22;
    hnamew[3]= loadText(font, hc_name[3], color, MRenderer);
    hscorew[3]= loadText(font, numToStr(hc_score[3]), color, MRenderer);
    hnamew[4]= loadText(font, hc_name[4], color, MRenderer);
    hscorew[4]= loadText(font, numToStr(hc_score[4]), color, MRenderer);
}

void writeScore()
{
    int l[5];
    for (int i=0; i<5; i++) l[i]= numToStr(hc_score[i]).length();

    for (int i=0; i<5; i++){
        Mapply_surface(478,245+i*87,hc_name[i].length()*20,40,hnamew[i]);
        Mapply_surface(945-l[i]*10,245+i*87,l[i]*20,40,hscorew[i]);
    }
}

void showScore(int* k)
{
    Mix_PlayChannel( -1, Mclick, 0 );

    int x, y;
    int recent=1;
    SDL_Event e;
    readScore(1);
    while (true){
        apply_BG(*k,BG);
        if (SDL_PollEvent(&e)!=0&&e.type==SDL_MOUSEBUTTONDOWN){
            x= e.button.x; y= e.button.y;
            if (x>=20&&x<=140&&y>=20&&y<=86) {
                Mix_PlayChannel( -1, Mclick, 0 );
                return;
            }
        }
        Mapply_surface(20,20,1160,640,hscore[recent]);
        Mapply_surface(20,20,120,66,back_button);
        writeScore();
        SDL_RenderPresent(MRenderer);
        SDL_Delay(3);
        *k= (*k+1) % 1360;
    }
}

void showTutorial(int* k)
{
    Mix_PlayChannel( -1, Mclick, 0 );
    int recent=0, x, y;
    SDL_Event e;
    while (true){
        apply_BG(*k,BG);
        if (SDL_PollEvent(&e)!=0&&e.type==SDL_MOUSEBUTTONDOWN){
            x= e.button.x; y= e.button.y;
            if (x>=20&&x<=140&&y>=20&&y<=86) { //back
                Mix_PlayChannel( -1, Mclick, 0 );
                return;
            }
            if ((x>=20&&x<=100&&y>=360&&y<=360+55&&recent) || (x>=1100&&x<=1180&&y>=360&&y<=360+55&&recent==0)){
                Mix_PlayChannel( -1, Mclick, 0 );
                recent= 1-recent;
            }
        }
        Mapply_surface(20,20,1160,640,tutor[recent]);
        if (recent) Mapply_surface(20,360,80,55,arrow[0]);
        else Mapply_surface(1100,360,80,55,arrow[1]);
        Mapply_surface(20,20,120,66,back_button);
        SDL_RenderPresent(MRenderer);
        SDL_Delay(3);
        *k= (*k+1) % 1360;
    }
}

void showQuit(int *k)
{
    Mix_PlayChannel( -1, Mclick, 0 );
    int x, y;
    SDL_Event e;
    while (true){
        apply_BG(*k,BG);
        Mapply_surface(228,25,743,198,Logo);
        for (int i=0; i<4; i++) Mapply_surface(486, 250+i*105, 228, 100, menu[i][status[i]]);
        Mapply_surface(1075,555,100,100,Mspeaker[Msound]);
        Mapply_surface(0,0,1200,680,quit);
        Mapply_surface(440,350,110,55,Y[0]);
        Mapply_surface(650,350,110,55,N[0]);
        if (SDL_PollEvent(&e)!=0&&e.type==SDL_MOUSEBUTTONDOWN){
            x= e.button.x; y= e.button.y;
            if (x>=440&&x<=550&&y>=350&&y<=405) {
                Mapply_surface(440,350,110,55,Y[1]);
                SDL_RenderPresent(MRenderer);
                Mix_PlayChannel( -1, Mclick, 0 );
                SDL_Delay(100);
                MquitSDL();
                exit(0);
            }
            if (x>=650&&x<=760&&y>=350&&y<=350+55) {
                Mapply_surface(650,350,110,55,N[1]);
                SDL_RenderPresent(MRenderer);
                Mix_PlayChannel( -1, Mclick, 0 );
                SDL_Delay(200);
                return;
            }
        }
        SDL_RenderPresent(MRenderer);
        SDL_Delay(3);
        *k= (*k+1) % 1360;
    }
}

void intro(SDL_Texture* T)
{
    for (int i=0; i<=255; i++)
    {
        SDL_RenderClear(MRenderer);
        SDL_SetTextureAlphaMod(T,i);
        Mapply_surface(0,0,1200,680,T);
        SDL_RenderPresent(MRenderer);
        SDL_Delay(6);
    }
    for (int i=255; i>0; i--) {
        SDL_RenderClear(MRenderer);
        SDL_SetTextureAlphaMod(T,i);
        Mapply_surface(0,0,1200,680,T);
        SDL_RenderPresent(MRenderer);
        SDL_Delay(2);
    }
    SDL_Delay(500);
}

void addButton(){
	menuButton[0].set(486, 714, 250,350, EVENT_PLAY);
	menuButton[1].set(486, 714, 355, 455, EVENT_SCORE);
	menuButton[2].set(486, 714, 460, 560, EVENT_TUTORIAL);
	menuButton[3].set(486, 714, 565, 665, EVENT_QUIT);
	menuButton[4].set(1075, 1175, 555, 655, EVENT_SOUND);
}


bool showMenu(bool trend)
{
    MinitSDL();
	addButton();
    int k=5;
    int pos;
    SDL_Event e;
    Mix_PlayMusic(menu_beat,-1);
    if (!Msound) Mix_PauseMusic();
    while (true){
        apply_BG(k,BG);
        Mapply_surface(228,25,743,198,Logo);
        if (SDL_PollEvent(&e)!=0){
            if (e.type==SDL_QUIT) break;
            if (e.type==SDL_MOUSEMOTION){
                for (int i=0; i<4; i++) status[i]= false;
                status[Position(e.button.x, e.button.y)]= true;
            }
            if (e.type==SDL_MOUSEBUTTONDOWN){
                pos= Position(e.button.x, e.button.y);
                switch (pos){
					case EVENT_PLAY: {
                        Mix_HaltMusic();
                        MquitSDL();
                        return 1;
                    }
					case EVENT_SCORE: {
                        showScore(&k);
                        break;
                    }
					case EVENT_TUTORIAL: {
                        showTutorial(&k);
                        break;
                    }
					case EVENT_QUIT: {
                        showQuit(&k);
                        break;
                    }
                    case EVENT_SOUND: {
                        MTransound(&k,Mspeaker[Msound],Mspeaker[!Msound]);
                        break;
                    }
                }
            }
        }
        for (int i=0; i<4; i++) Mapply_surface(486, 250+i*105, 228, 100, menu[i][status[i]]);
        Mapply_surface(1075,555,100,100,Mspeaker[Msound]);
        SDL_RenderPresent(MRenderer);
        SDL_Delay(3);
        k= (k+1) % 1360;
    }
    MquitSDL();
    return 0;
}