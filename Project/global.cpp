#include "globals.h"
#include "clTexture.h"
#include "clBoxHandler.h"
#include "clTimer.h"

//Gaming Window
SDL_Window* gWindow = NULL;

//Window Renderer
SDL_Renderer* gRenderer = NULL;

//Grid Sprites
clTexture gGridTexture;
//Box Sprites
clTexture gBoxSpriteSheetTexture;
SDL_Rect gSpriteClips[SPRITE_NUMBER];
clBoxHandler gBoxes[BOX_NUMBER];

//Text and Font variables
TTF_Font* gFontFPS = NULL;
TTF_Font* gFontPlayer = NULL;
clTexture gFPSTextTexture;
clTexture gTurnTextTexture;
clTexture gPlayerTextTexture;

//Sound Effects
Mix_Chunk* gSymbolSound = NULL;
Mix_Chunk* gVictorySound = NULL;

ePlayerTurn playerTurn = PLAYER_ONE;

/*************** FUNCTIONS ***************/

bool SDL_initialize()
{
	//Success flag
	bool fl_success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		cout << "SDL failed to initialize! SDL Error: " << SDL_GetError() << endl;
		fl_success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "Warning: Linear texture filtering not enabled!" << endl;
		}

		//Create Game Window
		gWindow = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window creation failed! SDL Error: " << SDL_GetError() << endl;
			fl_success = false;
		}
		else
		{
			//Create Renderer for the window (vsync ACTIVE)
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				cout << "Renderer creation failed! SDL Error: " << SDL_GetError() << endl;
				fl_success = false;
			}
			else
			{

				//PNG loading initialization
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "SDL_image failed to initialize! SDL_image Error: " << IMG_GetError() << endl;
					fl_success = false;
				}

				//Font loading initialization (SDL_ttf)
				if (TTF_Init() == -1)
				{
					cout << "SDL_ttf failed to initialize! SDL_ttf Error: " << TTF_GetError() << endl;
					fl_success = false;
				}

				//Audio loading initialization (SDL_mixer)
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					cout << "SDL_mixer failed to initialize! SDL_mixer Error: " << Mix_GetError() << endl;
					fl_success = false;
				}
			}
		}
	}

	return fl_success;
}

void SDL_close()
{
	//Free loaded images
	gBoxSpriteSheetTexture.freeTexture();

	//Free loaded text textures
	gFPSTextTexture.freeTexture();
	gPlayerTextTexture.freeTexture();
	gGridTexture.freeTexture();

	//Free global font
	TTF_CloseFont(gFontFPS);
	TTF_CloseFont(gFontPlayer);
	gFontFPS = NULL;
	gFontPlayer = NULL;

	//Free the sound effects
	Mix_FreeChunk(gSymbolSound);
	Mix_FreeChunk(gVictorySound);
	gSymbolSound = NULL;
	gVictorySound = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool load_Game_Data()
{
	//Success flag
	bool fl_success = true;

	//Load sprites
	if (!gBoxSpriteSheetTexture.createFromFile(gRenderer, "data/symbols.png"))
	{
		cout << "X and O sprites loading failed!" << endl;
		fl_success = false;
	}
	else
	{

		//Set standard alpha for mouse over hints
		gBoxSpriteSheetTexture.setBlendMode(SDL_BLENDMODE_BLEND);

		//Set O and X sprites
		for (int i = 0; i < SPRITE_NUMBER; ++i)
		{
			gSpriteClips[i].x = i * BOX_SIZE;
			gSpriteClips[i].y = 0;
			gSpriteClips[i].w = BOX_SIZE;
			gSpriteClips[i].h = BOX_SIZE;
		}

		//Set top left point for each box
		gBoxes[0].setPosition(MARGIN, MARGIN);
		gBoxes[1].setPosition(MARGIN + BOX_SIZE + LINE_THICKNESS, MARGIN);
		gBoxes[2].setPosition(MARGIN + 2 * BOX_SIZE + 2 * LINE_THICKNESS, MARGIN);
		gBoxes[3].setPosition(MARGIN, MARGIN + BOX_SIZE + LINE_THICKNESS);
		gBoxes[4].setPosition(MARGIN + BOX_SIZE + LINE_THICKNESS, MARGIN + BOX_SIZE + LINE_THICKNESS);
		gBoxes[5].setPosition(MARGIN + 2 * BOX_SIZE + 2 * LINE_THICKNESS, MARGIN + BOX_SIZE + LINE_THICKNESS);
		gBoxes[6].setPosition(MARGIN, MARGIN + 2 * BOX_SIZE + 2 * LINE_THICKNESS);
		gBoxes[7].setPosition(MARGIN + BOX_SIZE + LINE_THICKNESS, MARGIN + 2 * BOX_SIZE + 2 * LINE_THICKNESS);
		gBoxes[8].setPosition(MARGIN + 2 * BOX_SIZE + 2 * LINE_THICKNESS, MARGIN + 2 * BOX_SIZE + 2 * LINE_THICKNESS);
	}

	//Load grid
	if (!gGridTexture.createFromFile(gRenderer, "data/grid.png"))
	{
		cout << "Grid image loading failed!" << endl;
		fl_success = false;
	}

	//Open the fonts
	gFontFPS = TTF_OpenFont("data/Quicksand.ttf", 12);
	if (gFontFPS == NULL)
	{
		cout << "Failed to load Bebas font! SDL_ttf Error: " << TTF_GetError() << endl;
		fl_success = false;
	}

	gFontPlayer = TTF_OpenFont("data/OrangeJuice.ttf", 80);
	if (gFontPlayer == NULL)
	{
		cout << "Failed to load OrangeJuice font! SDL_ttf Error: " << TTF_GetError() << endl;
		fl_success = false;
	}
	else
	{
		//Set text color as black
		SDL_Color textColor = { 0, 0, 0, 255 };

		//Load prompt texture
		if (!gTurnTextTexture.createFromText(gRenderer, gFontPlayer, "Turn:", textColor))
		{
			cout << "Unable to render prompt texture!" << endl;
			fl_success = false;
		}
	}

	//Load sound effects
	gSymbolSound = Mix_LoadWAV("data/pop.wav");
	if (gSymbolSound == NULL)
	{
		cout << "Failed to load pop sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		fl_success = false;
	}
	else
	{
		Mix_VolumeChunk(gSymbolSound, 5);
	}
	gVictorySound = Mix_LoadWAV("data/victory.wav");
	if (gVictorySound == NULL)
	{
		cout << "Failed to load victory sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		fl_success = false;
	}
	else
	{
		Mix_VolumeChunk(gVictorySound, 10);
	}

	return fl_success;
}

float RoundDown(float number, const int value)
{
	// EXAMPLE with 2 digits:
	// 12.3456 * 100 = 1234.56 
	// 1234.56 + .5 = 1235.06		for rounding off value 
	// then type cast to int so value is 1235
	// then divided by 100 so the value converted into 12.35 

	number = (int)(number * value + .5);
	return (number / value);
}

void draw_grid()
{
	// Old Grid realized with rectangles
	/*
	SDL_Rect gridLine;
	SDL_SetRenderDrawColor(gRenderer, 50, 50, 50, 0xFF);
	//vertical left
	gridLine = { (MARGIN + BOX_SIZE), MARGIN, LINE_THICKNESS, GRID_SIZE };
	SDL_RenderFillRect(renderer, &gridLine);
	//vertical right
	gridLine = { (MARGIN + 2 * BOX_SIZE + LINE_THICKNESS), MARGIN, LINE_THICKNESS, GRID_SIZE };
	SDL_RenderFillRect(renderer, &gridLine);
	//horizontal up
	gridLine = { MARGIN, MARGIN + BOX_SIZE, GRID_SIZE , LINE_THICKNESS };
	SDL_RenderFillRect(renderer, &gridLine);
	//horizontal up
	gridLine = { MARGIN, (MARGIN + 2 * BOX_SIZE + LINE_THICKNESS), GRID_SIZE , LINE_THICKNESS };
	SDL_RenderFillRect(renderer, &gridLine);
	*/

	// New Grid realized with texture
	gGridTexture.render(gRenderer, MARGIN - 3, MARGIN + 3);
}

bool endgameCondition(eWinner& winner)
{
	bool flag_gameover = false;

	// Check for winner status
	if (checkTris(0, 1, 2) || checkTris(3, 4, 5) || checkTris(6, 7, 8) || // horizontal tris
		checkTris(0, 3, 6) || checkTris(1, 4, 7) || checkTris(2, 5, 8) || // vertical tris
		checkTris(0, 4, 8) || checkTris(2, 4, 6))						  // diagonal tris
	{
		flag_gameover = true;
		Mix_PlayChannel(-1, gVictorySound, 0);

		//wins the player who just finished his turn
		//if it's player 1 turn... that means player 2 won
		if (playerTurn == PLAYER_ONE)
		{
			winner = P2_WINNER;
			return flag_gameover;
		}
		if (playerTurn == PLAYER_TWO)
		{
			winner = P1_WINNER;
			return flag_gameover;
		}
	}

	// Check for no winner status
	bool flag_allboxused = true;

	for (int i = 0; i < BOX_NUMBER && flag_allboxused; i++)
	{
		if (!gBoxes[i].isUsed())
		{
			flag_allboxused = false;
		}
	}

	if (flag_allboxused)
	{
		winner = NO_WINNER;
		flag_gameover = true;
	}

	return flag_gameover;
}

bool checkTris(const int first, const int second, const int third)
{
	bool flagTris = false;

	if (gBoxes[first].getCurrentSprite() == gBoxes[second].getCurrentSprite() &&
		gBoxes[second].getCurrentSprite() == gBoxes[third].getCurrentSprite() &&
		gBoxes[first].isUsed() && gBoxes[second].isUsed() && gBoxes[third].isUsed())
	{
		flagTris = true;
	}

	return flagTris;
}