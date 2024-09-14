#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
	SDL_Window* window = NULL;

	SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			if (!IMG_Init(IMG_INIT_PNG))
			{
				printf("FAIL!");

				return 0;
			}

			screenSurface = SDL_GetWindowSurface(window);

			//Fill the surface white
			// SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xAF, 0xAF, 0xAF));

			//Update the surface
			// SDL_UpdateWindowSurface(window);

			SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "Assets/character_big.png");
			if (playerTexture == NULL)
			{
				printf("NULL PLAYER TEXTURE");
			}

			SDL_Rect position = {100, 100, 64, 64};

			SDL_Event event;
			bool quit = false;

			while (quit == false)
			{
				SDL_SetRenderDrawColor(renderer, 0, 204, 0, 255);
				SDL_RenderClear(renderer);

				SDL_Rect size;
				size.w = 64;
				size.h = 64;


				/*
				if (SDL_RenderCopy(renderer, playerTexture, NULL, &position) < 0)
				{
					printf("ERROR: %s", SDL_GetError());
				}
				SDL_RenderPresent(renderer);
				*/

				while (SDL_PollEvent(&event)) 
				{ 
					if (event.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (event.type == SDL_KEYDOWN)
					{
						switch (event.key.keysym.sym)
						{
						case SDLK_UP:
							printf("UP\n");
							position.y -= 5;

							if (SDL_QueryTexture(playerTexture, NULL, NULL, &size.w, &size.h) < 0)
							{
								printf("ERROR: %s", SDL_GetError());
							}

							if (SDL_RenderCopy(renderer, playerTexture, NULL, &position) < 0)
							{
								printf("ERROR: %s", SDL_GetError());
							}
							SDL_RenderPresent(renderer);
							break;
						case SDLK_DOWN:
							printf("DOWN\n");
							position.y += 5;

							if (SDL_QueryTexture(playerTexture, NULL, NULL, &size.w, &size.h) < 0)
							{
								printf("ERROR: %s", SDL_GetError());
							}
							if (SDL_RenderCopy(renderer, playerTexture, NULL, &position) < 0)
							{
								printf("ERROR: %s", SDL_GetError());
							}
							SDL_RenderPresent(renderer);
							break;
						case SDLK_LEFT:
							printf("LEFT\n");
							position.x -= 5;

							if (SDL_QueryTexture(playerTexture, NULL, NULL, &size.w, &size.h) < 0)
							{
								printf("ERROR: %s", SDL_GetError());
							}
							if (SDL_RenderCopy(renderer, playerTexture, NULL, &position) < 0)
							{
								printf("ERROR: %s", SDL_GetError());
							}
							SDL_RenderPresent(renderer);
							break;
						case SDLK_RIGHT:
							printf("RIGHT\n");
							position.x += 5;

							if (SDL_QueryTexture(playerTexture, NULL, NULL, &size.w, &size.h) < 0)
							{
								printf("ERROR: %s", SDL_GetError());
							}
							if (SDL_RenderCopy(renderer, playerTexture, NULL, &position) < 0)
							{
								printf("ERROR: %s", SDL_GetError());
							}
							SDL_RenderPresent(renderer);
							break;
						default:
							printf("You pressed a different key");
							break;
						}
					}
					else
					{
						if (SDL_QueryTexture(playerTexture, NULL, NULL, &size.w, &size.h) < 0)
						{
							printf("ERROR: %s", SDL_GetError());
						}
						if (SDL_RenderCopy(renderer, playerTexture, NULL, &position) < 0)
						{
							printf("ERROR: %s", SDL_GetError());
						}
						SDL_RenderPresent(renderer);
					}
				} 
				
				SDL_Delay(16);
			}
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
