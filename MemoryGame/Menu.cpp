#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Menu.h"

using namespace std;

const int WIDTH= 1200;
const int HEIGHT= 680;
const string TITLE= "Memory Game!!";

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
SDL_Renderer* MRenderer;
SDL_Texture *Logo, *BG, *BG2, *back_button, *quit, *prePlay;
SDL_Texture *menu[4][2], *tutor[2], *arrow[2], *Y[2], *N[2], *hscore, *easy[2], *med[2], *hard[2], *play[2];
SDL_Texture *Mspeaker[2];
bool status[6]={false}, Msound=1;
string hc_name[5];
int hc_score[5];
SDL_Texture *hnamew[5], *hscorew[5];
TTF_Font* font;
Mix_Music* menu_beat;
Mix_Chunk* Mclick;

SDL_Texture* MloadTexture(string path)
{
    SDL_Texture* newTexture= NULL;
    SDL_Surface* loadedSurface= IMG_Load(path.c_str());
    if (loadedSurface==NULL) {cout<<"Error load image\n"; exit(1);}
    newTexture= SDL_CreateTextureFromSurface(MRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

SDL_Texture* loadText(string text, SDL_Color color)
{
    SDL_Texture* newTexture= NULL;
    SDL_Surface* textSurface= TTF_RenderText_Blended(font, text.c_str(), color);
    if (textSurface==NULL) {cout<<"Error font\n"; exit(1);}
    newTexture= SDL_CreateTextureFromSurface(MRenderer, textSurface);
    SDL_FreeSurface(textSurface);
    return newTexture;
}

void MloadImage()
{
    Logo= MloadTexture("menu/Game_Logo.PNG");
    BG= MloadTexture("menu/BG.PNG");
    back_button= MloadTexture("menu/back.PNG");
    quit= MloadTexture("menu/quit.PNG");
    prePlay= MloadTexture("menu/game_mode.PNG");

    menu[0][0]= MloadTexture("menu/PLAY.PNG");
    menu[0][1]= MloadTexture("menu/PLAY_s.PNG");
    menu[1][0]= MloadTexture("menu/SCORE.PNG");
    menu[1][1]= MloadTexture("menu/SCORE_s.PNG");
    menu[2][0]= MloadTexture("menu/HELP.PNG");
    menu[2][1]= MloadTexture("menu/HELP_s.PNG");
    menu[3][0]= MloadTexture("menu/EXIT.PNG");
    menu[3][1]= MloadTexture("menu/EXIT_s.PNG");
    Mspeaker[0]= MloadTexture("menu/mute.PNG");
    Mspeaker[1]= MloadTexture("menu/speaker.PNG");
    tutor[0] = MloadTexture("menu/TUTOR1.PNG");
    tutor[1] = MloadTexture("menu/TUTOR2.PNG");
    arrow[0] = MloadTexture("menu/arrow1.PNG");
    arrow[1] = MloadTexture("menu/arrow2.PNG");
    hscore= MloadTexture("menu/highscore.PNG");
    Y[0]= MloadTexture("menu/YES.PNG");
    Y[1]= MloadTexture("menu/YES_s.PNG");
    N[0]= MloadTexture("menu/NO.PNG");
    N[1]= MloadTexture("menu/NO_s.PNG");
    play[0]= MloadTexture("menu/playnow.PNG");
    play[1]= MloadTexture("menu/playnow_s.PNG");
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

int Position(int x_, int y_)
{
    if (x_>=1075&&x_<=1175&&y_>=555&&y_<=655) return 4;
    if (x_<=486||x_>=486+228) return 5;
    if (y_>=250&&y_<=350) return 0;
    if (y_>=355&&y_<=455) return 1;
    if (y_>=460&&y_<=560) return 2;
    if (y_>=565&&y_<=665) return 3;
    return 5;
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

string numToSt(int k)
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

void readScore()
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
	
    hnamew[0]= loadText(hc_name[0], color);
    hscorew[0]= loadText(numToSt(hc_score[0]), color);
	
	color.r = 126;
	color.g = 102;
	color.b = 102;
    hnamew[1]= loadText(hc_name[1], color);
    hscorew[1]= loadText(numToSt(hc_score[1]), color);
	
	color.r = 139;
	color.g = 57;
	color.b = 7;
	hnamew[2]= loadText(hc_name[2], color);
    hscorew[2]= loadText(numToSt(hc_score[2]), color);

	color.r = 22;
	color.g = 168;
	color.b = 22;
    hnamew[3]= loadText(hc_name[3], color);
    hscorew[3]= loadText(numToSt(hc_score[3]), color);
    hnamew[4]= loadText(hc_name[4], color);
    hscorew[4]= loadText(numToSt(hc_score[4]), color);
}

void writeScore()
{
    int l[5];
    for (int i=0; i<5; i++) l[i]= numToSt(hc_score[i]).length();

    for (int i=0; i<5; i++){
        Mapply_surface(478,245+i*87,hc_name[i].length()*20,40,hnamew[i]);
        Mapply_surface(945-l[i]*10,245+i*87,l[i]*20,40,hscorew[i]);
    }
}

void showScore(int* k)
{
    Mix_PlayChannel( -1, Mclick, 0 );
    int x, y;
    SDL_Event e;
    readScore();
    while (true){
        apply_BG(*k,BG);
        if (SDL_PollEvent(&e)!=0&&e.type==SDL_MOUSEBUTTONDOWN){
            x= e.button.x; y= e.button.y;
            if (x>=20&&x<=140&&y>=20&&y<=86) {
                Mix_PlayChannel( -1, Mclick, 0 );
                return;
            }
        }
        Mapply_surface(20,20,1160,640,hscore);
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

bool showMenu(bool trend)
{
    MinitSDL();
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
                    case 0: {
                        MquitSDL();
                        return 1;
                        break;
                    }
                    case 1: {
                        showScore(&k);
                        break;
                    }
                    case 2: {
                        showTutorial(&k);
                        break;
                    }
                    case 3: {
                        showQuit(&k);
                        break;
                    }
                    case 4: {
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
