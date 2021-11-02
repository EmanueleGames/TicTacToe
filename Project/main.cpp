#include <iostream>
#include <string>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "globals.h"
#include "clTexture.h"
#include "clBoxHandler.h"
#include "clTimer.h"

using namespace std;

int main (int argc, char* argv[])
{
	//SLD components initialization
	if (!SDL_initialize())
	{
		cout << "SDL initialization Failed!" << endl;
	}
	else
	{
		//Load game assets
		if (!load_Game_Data())
		{
			cout << "Failed to load media!" << endl;
		}
		else
		{
			//Main loop flag
			bool quitGame = false;
			//Match over flag
			bool matchOver = false;
			eWinner winner = NO_WINNER;

			//Event handler
			SDL_Event event;

			//Text rendering variables
			stringstream textFPS;
			stringstream textPlayerTurn;
			SDL_Color textColor = { 0, 0, 0, 255 };

			//FPS Timer and variables
			float avgFPS = 0;
			int countedFrames = 0;
			int decimalDigitValue = 1;
			for (int i = 0; i < FPS_DECIMAL_DIGIT; i++)
			{
				decimalDigitValue *= 10;
			}
			clTimer fpsTimer;
			fpsTimer.start();


			//Game main loop
			while (!quitGame)
			{
				//Handle events on queue 
				while (SDL_PollEvent(&event) != 0)
				{
					//User clicks X on the window (close)
					if (event.type == SDL_QUIT)
					{
						quitGame = true;
					}
					
					//Handle boxes events
					for (int i = 0; i < BOX_NUMBER; i++)
					{
						gBoxes[i].handleMouseEvent(&event, gSymbolSound);
					}
				}

				//Check if the match is over
				matchOver = endgameCondition(winner);

				//Match over screen
				while (!quitGame && matchOver)
				{
					//Handle events on queue 
					while (SDL_PollEvent(&event) != 0)
					{
						//User clicks X on the window (close)
						if (event.type == SDL_QUIT)
						{
							quitGame = true;
						}
						//Handle key presses
						else if (event.type == SDL_KEYDOWN)
						{
							//Game starts over with SPACEBAR
							if (event.key.keysym.sym == SDLK_SPACE)
							{
								for (int i = 0; i < BOX_NUMBER; i++)
								{
									gBoxes[i].resetBox();
								}
								matchOver = false;
								playerTurn = PLAYER_ONE;
							}
						}
					}

					//Clear the window (white color)
					SDL_SetRenderDrawColor(gRenderer, 240, 240, 240, 255);
					SDL_RenderClear(gRenderer);

					textPlayerTurn.str("");
					if (winner == P1_WINNER)   
						textPlayerTurn << "P1 won! Press SPACE to play again";
					if (winner == P2_WINNER)
						textPlayerTurn << "P2 won! Press SPACE to play again";
					if (winner == NO_WINNER)
						textPlayerTurn << "DRAW! Press SPACE to play again";

					//Winner text
					if (!gPlayerTextTexture.createFromText(gRenderer, gFontPlayer, textPlayerTurn.str().c_str(), textColor))
					{
						cout << "Unable to render player texture!" << endl;
					}

					gPlayerTextTexture.render(gRenderer, (WINDOW_WIDTH - gPlayerTextTexture.getWidth()) / 2, (WINDOW_HEIGHT - gPlayerTextTexture.getHeight()) / 2);

					//Switch front and back buffer
					SDL_RenderPresent(gRenderer);

					++countedFrames;
				} // end of match over screen

				//Calculate and correct fps
				avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
				if (avgFPS > 2000000)
				{
					avgFPS = 0;
				}
				else
				{	
					avgFPS = RoundDown(avgFPS, decimalDigitValue);
				}

				//FPS text
				textFPS.str("");
				textFPS << "Average FPS (vsync):  " << avgFPS ;
				if (!gFPSTextTexture.createFromText(gRenderer, gFontFPS, textFPS.str().c_str(), textColor))
				{
					cout << "Unable to render FPS texture!" << endl;
				}

				//Player Turn text
				textPlayerTurn.str("");
				if (playerTurn == PLAYER_ONE)
					textPlayerTurn << "Player 1";
				else
					textPlayerTurn << "Player 2";

				if (!gPlayerTextTexture.createFromText(gRenderer, gFontPlayer, textPlayerTurn.str().c_str(), textColor))
				{
					cout << "Unable to render player turn texture!" << endl;
				}

				//Clear the window (white color)
				SDL_SetRenderDrawColor(gRenderer, 240, 240, 240, 255);
				SDL_RenderClear(gRenderer);

				//Draw the game grid
				draw_grid();

				//Render boxes
				for (int i = 0; i < BOX_NUMBER; i++)
				{
					gBoxes[i].render(gRenderer, gBoxSpriteSheetTexture, gSpriteClips);
				}

				//Render text
				gFPSTextTexture.render(gRenderer, 2, 0);
				gTurnTextTexture.render(gRenderer, (WINDOW_WIDTH + MARGIN + GRID_SIZE - gTurnTextTexture.getWidth()) / 2, (WINDOW_HEIGHT / 2 - gTurnTextTexture.getHeight() / 2));
				gPlayerTextTexture.render(gRenderer, (WINDOW_WIDTH + MARGIN + GRID_SIZE - gPlayerTextTexture.getWidth()) / 2, (WINDOW_HEIGHT / 2 + gPlayerTextTexture.getHeight() / 2 ));

				//Switch front and back buffer
				SDL_RenderPresent(gRenderer);

				countedFrames++;
			}
		}
	}

	//Free memory and close SDL
	SDL_close();

	//system("PAUSE");
	return 0;
}
