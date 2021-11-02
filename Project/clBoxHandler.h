#ifndef CL_BOX_HANDLER_H
#define CL_BOX_HANDLER_H

#include "constants.h"
#include "globals.h"
#include "clTexture.h"

enum eBoxStatus {
	BOX_EMPTY		 = 0,
	BOX_MARKED_X	 = 1,
	BOX_MARKED_O	 = 2,
	BOX_STATUS_COUNT = 3
};

class clTexture;

class clBoxHandler
{

	public:
		//Constructor
		clBoxHandler();
		//Destructor
		//~clBoxHandler();
		
		//Sets top left position
		void setPosition(int x, int y);

		//Handles mouse event
		void handleMouseEvent(SDL_Event* event, Mix_Chunk* popSound);
		
		//Shows box sprite
		void render(SDL_Renderer* renderer, clTexture &textureSpriteSheet, SDL_Rect* spriteClips);

		bool isUsed() const;
		eBoxStatus getCurrentSprite() const;

		void resetBox();
		
	private:

		//Flag indicating if the box has been claimed by any player
		bool used;

		//Top left corner
		SDL_Point mBoxTopLeftCorner;

		//Currently showed sprite
		eBoxStatus mCurrentSprite;
		//Currently showed sprite transparency for player hints
		Uint8 mCurrentSpriteAlpha;
};

#endif