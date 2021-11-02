#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "constants.h"

class clTexture;
class clBoxHandler;
class clTimer;

//Gaming Window
extern SDL_Window* gWindow;

//Window Renderer
extern SDL_Renderer* gRenderer;

//Grid Sprites
extern clTexture gGridTexture;
//Box Sprites
extern clTexture gBoxSpriteSheetTexture;
extern SDL_Rect gSpriteClips[SPRITE_NUMBER];

//Box objects
extern clBoxHandler gBoxes[BOX_NUMBER];

//Font objects
extern TTF_Font* gFontFPS;
extern TTF_Font* gFontPlayer;
extern clTexture gFPSTextTexture;
extern clTexture gTurnTextTexture;
extern clTexture gPlayerTextTexture;

//The sound effects that will be used
extern Mix_Chunk* gSymbolSound;
extern Mix_Chunk* gVictorySound;

extern ePlayerTurn playerTurn;

/*************** FUNCTIONS ***************/

bool SDL_initialize();		//Starts up SDL and creates window
void SDL_close();			//Frees media and shuts down SDL
bool load_Game_Data();		//Loads media

float RoundDown(float number, const int value);
void draw_grid();
bool endgameCondition(eWinner &winner);
bool checkTris(const int first, const int second, const int third);

#endif