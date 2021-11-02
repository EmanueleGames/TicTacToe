#include "clBoxHandler.h"

clBoxHandler::clBoxHandler()
{
	mBoxTopLeftCorner.x = 0;
	mBoxTopLeftCorner.y = 0;
	used = false;

	mCurrentSprite = BOX_EMPTY;
	mCurrentSpriteAlpha = 255;
}

void clBoxHandler::setPosition(int x, int y)
{
	mBoxTopLeftCorner.x = x;
	mBoxTopLeftCorner.y = y;
}

void clBoxHandler::handleMouseEvent(SDL_Event* event, Mix_Chunk* popSound)
{
	//Catch Mouse Click and Mouse Over events
	if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN)
	{
		//Get mouse position
		int xMouse, yMouse;
		SDL_GetMouseState(&xMouse, &yMouse);

		//Check if mouse is inside the box
		bool flag_inside = true;

		//Mouse is left of the box
		if (xMouse < mBoxTopLeftCorner.x)
		{
			flag_inside = false;
		}
		//Mouse is right of the box
		else if (xMouse > mBoxTopLeftCorner.x + BOX_SIZE)
		{
			flag_inside = false;
		}
		//Mouse above the box
		else if (yMouse < mBoxTopLeftCorner.y)
		{
			flag_inside = false;
		}
		//Mouse below the box
		else if (yMouse > mBoxTopLeftCorner.y + BOX_SIZE)
		{
			flag_inside = false;
		}

		//Mouse is outside box
		if (!flag_inside)
		{
			if (!used)	// if the box has not been claimed, show empty texture
				mCurrentSprite = BOX_EMPTY;
		}

		//Mouse is inside the box
		else
		{
			//Set mouse over sprite
			switch (event->type)
			{
			case SDL_MOUSEMOTION:
				if (!used)	// if the box has not been claimed, show empty texture
				{
					if (playerTurn == PLAYER_ONE)
					{
						mCurrentSprite = BOX_MARKED_X;
					}
					else
					{
						mCurrentSprite = BOX_MARKED_O;
					}
					mCurrentSpriteAlpha = 80;

				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (!used)	// if the box has not been claimed, show empty texture
				{

					if (playerTurn == PLAYER_ONE)
					{
						mCurrentSprite = BOX_MARKED_X;
						playerTurn = PLAYER_TWO;
					}
					else
					{
						mCurrentSprite = BOX_MARKED_O;
						playerTurn = PLAYER_ONE;
					}
					Mix_PlayChannel(-1, popSound, 0);
					mCurrentSpriteAlpha = 255;
					used = true;

				}
				break;
			}
		}
	}
}

void clBoxHandler::render(SDL_Renderer* renderer, clTexture &textureSpriteSheet, SDL_Rect* spriteClips)
{
	if (mCurrentSprite != BOX_EMPTY)
	{
		//Show current box sprite
		if (mCurrentSprite > 0)
		{
			textureSpriteSheet.setAlphaModulation(mCurrentSpriteAlpha);
			textureSpriteSheet.render(renderer, mBoxTopLeftCorner.x, mBoxTopLeftCorner.y, &spriteClips[mCurrentSprite-1]);
		}
	}
}

bool clBoxHandler::isUsed() const
{
	return used;
}

eBoxStatus clBoxHandler::getCurrentSprite() const
{
	return mCurrentSprite;
}

void clBoxHandler::resetBox()
{
	used = false;
	mCurrentSprite = BOX_EMPTY;
}