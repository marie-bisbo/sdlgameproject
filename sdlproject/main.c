#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "constants.h"
#include "maths.h"
#include "app.h"
#include "entity.h"

int main(int argc, char* args[])
{
	int timeSinceStart = 0;
	int deltaTime = 0;

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
	SDL_Rect position = { entity->xPosition, entity->yPosition, entity->width, entity->height };
	entity->texture = playerTexture;
	entity->collider = position;


	SDL_Event event;
	bool quit = false;

	double currentPosition = 0.f;
	double currentVelocity = 0.f;

	SDL_Rect size;
	size.w = 64;
	size.h = 64;

	SDL_Rect floor;
	floor.x = 100;
	floor.y = 600;
	floor.h = 50;
	floor.w = 300;

	bool movingUp = false;

	while (false == quit)
	{
		deltaTime = SDL_GetTicks64() - timeSinceStart;
		timeSinceStart = SDL_GetTicks64();
		double deltaSeconds = deltaTime * 0.001;

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
					movingUp = true;
					break;
				case SDLK_DOWN:
					printf("DOWN\n");
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
					break;
				default:
					printf("You pressed a different key\n");
					break;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					movingUp = false;
					printf("UP Released\n");
					break;
				}
			}
		}

		SDL_RenderClear(app->renderer);
		SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(app->renderer, &floor);

		// Draw floor
		SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);

		if (movingUp)
		{
			currentVelocity += THRUST * deltaSeconds;
		}

		currentVelocity += GRAVITY * deltaSeconds;
		currentPosition += currentVelocity * deltaSeconds;
		if (Absolute(currentPosition) >= 1 && !Colliding(&entity->collider, &floor))
		{
			int toMove = currentPosition > 0 ? 1 : -1;
			position.y += toMove;
			entity->yPosition = position.y;
			currentPosition -= toMove;
			entity->collider = position;
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
