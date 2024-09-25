#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "constants.h"
#include "maths.h"
#include "app.h"
#include "entity.h"

#define SDL_HINT_RENDER_VSYNC 1

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

	SDL_Rect position = { entity->xPosition, entity->yPosition, entity->width, entity->height };

	SDL_Event event;
	bool quit = false;

	double currentPosition = 0.f;
	double currentVelocity = 0.f;

	SDL_Rect size;
	size.w = 64;
	size.h = 64;

	bool jumping = true;

	while (false == quit)
	{
		deltaTime = SDL_GetTicks64() - timeSinceStart;
		timeSinceStart = SDL_GetTicks64();

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
					// position.y -= 1;
					// currentVelocity += JUMP_VELOCITY;
					break;
				default:
					printf("You pressed a different key\n");
					break;
				}

				entity->xPosition = position.x;
				entity->yPosition = position.y;
			}
		}

		SDL_SetRenderDrawColor(app->renderer, 0, 204, 0, 255);
		SDL_RenderClear(app->renderer);

		double deltaSeconds = deltaTime * 0.001;
		currentVelocity += GRAVITY * deltaSeconds;
		// currentPosition += (currentVelocity * deltaSeconds) + (0.5 * GRAVITY * deltaSeconds * deltaSeconds);
		currentPosition += currentVelocity * deltaSeconds;
		// if ((Absolute(currentPosition) >= 1.f && entity->yPosition < 600))
		if (entity->yPosition < 600)
		{
			// int amountToMove = (int)currentPosition;
			position.y += round(currentPosition);
			printf("Position: %d\n", position.y);
			// position.y += amountToMove;
			entity->yPosition = position.y;
			// currentPosition -= amountToMove;
		}
		else if (entity->yPosition >= 600)
		{
			deltaSeconds = 0;
			currentVelocity = 0;
			currentPosition = 0;
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
