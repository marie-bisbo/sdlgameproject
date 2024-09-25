#pragma once

#include <SDL.h>
#include <SDL_image.h>

typedef struct _Entity
{
	int xPosition;
	int yPosition;
	int width;
	int height;
	SDL_Texture* texture;
} Entity;

