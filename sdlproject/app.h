#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include "constants.h"

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

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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


