#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "constants.h"
#include "maths.h"

typedef struct _Entity
{
	int xPosition;
	int yPosition;
	int width;
	int height;
	SDL_Texture* texture;
} Entity;

typedef struct _App
{
	SDL_Window* window;
	SDL_Renderer* renderer;
} App;

App* InitApp()
{
	App* app = malloc(sizeof(App));
	if (NULL == app)
	{
		printf("Failed to allocate memory for app\n");

		return NULL;
	}

	bool initFailed = false;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

		initFailed = true;
	}
	else
	{
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (NULL == window)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());

			initFailed = true;
		}


		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (NULL == renderer)
		{
			printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());

			initFailed = true;
		}
	}	

	if (initFailed)
	{
		free(app);
		app = NULL;

		return NULL;
	}

	app->window = window;
	app->renderer = renderer;

	return app;
}

int main(int argc, char* args[])
{
	int timeSinceStart = 0;
	int deltaTime = 0;
	int requiredDeltaTime = 1000 / MAX_FRAMERATE;

	App* app = InitApp();
	if (NULL == app)
	{
		printf("Failed to initialise app\n");

		return 0;
	}

	Entity* entity = malloc(sizeof(Entity));
	if (NULL == entity)
	{
		printf("Failed to allocate memory for entity\n");

		return 0;
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf("Failed to initialise PNG\n");

		return 0;
	}

	SDL_Texture* playerTexture = IMG_LoadTexture(app->renderer, "Assets/character_big.png");
	if (NULL == playerTexture)
	{
		printf("NULL PLAYER TEXTURE\n");
	}

	entity->xPosition = 100;
	entity->yPosition = 100;
	entity->width = 64;
	entity->height = 64;
	entity->texture = playerTexture;

	SDL_Rect position = {entity->xPosition, entity->yPosition, entity->width, entity->height};

	SDL_Event event;
	bool quit = false;

	bool jumping = false;

	double currentPosition = 0.f;
	double timeSinceMove = 0.f;
	double currentVelocity = 0.f;

	while (false == quit)
	{
		deltaTime = SDL_GetTicks64() - timeSinceStart;
		timeSinceStart = SDL_GetTicks64();

		/* Why is this not working!? !? !? !
		if (deltaTime < requiredDeltaTime)
		{
			SDL_Delay(requiredDeltaTime - deltaTime);
			deltaTime = requiredDeltaTime;
			printf("Delta time: %d\n", deltaTime);
			printf("Time since start: %d\n", timeSinceStart);
		}
		*/

		SDL_SetRenderDrawColor(app->renderer, 0, 204, 0, 255);
		SDL_RenderClear(app->renderer);

		SDL_Rect size;
		size.w = 64;
		size.h = 64;

		double deltaSeconds = deltaTime * 0.001;
		timeSinceMove += deltaSeconds;
		currentVelocity += GRAVITY * timeSinceMove;
		currentPosition = (currentVelocity * timeSinceMove) + (0.5 * GRAVITY * timeSinceMove * timeSinceMove);
		if (Absolute(currentPosition) >= 1.f && entity->yPosition < 600)
		{
			int amountToMove = (int)currentPosition;
			printf("Amount to move: %d\n", amountToMove);
			position.y += amountToMove;
			entity->yPosition = position.y;
			// Carry over fractional component 
			currentPosition = currentPosition - amountToMove;
			timeSinceMove = 0;
		}
		else if (entity->yPosition >= 600)
		{
			deltaSeconds = 0;
			timeSinceMove = 0;
			currentVelocity = 0;
			currentPosition = 0;
		}
		if (jumping)
		{
			position.y -= 1;
			entity->yPosition = position.y;
			currentVelocity += JUMP_VELOCITY;
			jumping = false;
		}

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
					break;
				case SDLK_DOWN:
					printf("DOWN\n");
					position.y += 5;
					break;
				case SDLK_LEFT:
					printf("LEFT\n");
					position.x -= 5;
					break;
				case SDLK_RIGHT:
					printf("RIGHT\n");
					position.x += 5;
					break;
				case SDLK_SPACE:
					printf("SPACE\n");
					jumping = true;
					break;
				default:
					printf("You pressed a different key\n");
					break;
				}

				entity->xPosition = position.x;
				entity->yPosition = position.y;
			}
		} 

		if (SDL_QueryTexture(playerTexture, NULL, NULL, &size.w, &size.h) < 0)
		{
			printf("ERROR: %s\n", SDL_GetError());
		}
		if (SDL_RenderCopy(app->renderer, playerTexture, NULL, &position) < 0)
		{
			printf("ERROR: %s\n", SDL_GetError());
		}
		SDL_RenderPresent(app->renderer);
	}

	SDL_DestroyWindow(app->window);

	SDL_Quit();

	return 0;
}
