/*
	PC - Implementation using SDL

	Not part of the emulator core. This file implements
	the functions in graphics.h but its implementation 
	depends on the device which is running the emulator.
*/

#include "graphics.h"

#include <SDL.h>

const uint8_t MODIFIER = 5;
uint16_t SCREEN_WIDTH = LCD_WIDTH;
uint16_t SCREEN_HEIGHT = LCD_HEIGHT;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool GFX_init(void) {
	// Initialization flag
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error - SDL_INIT_VIDEO\n");
		success = false;
	}
	else {
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * MODIFIER, SCREEN_HEIGHT * MODIFIER, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created!\n");
			success = false;
		}
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
	SDL_Delay(1000);
	return success;
}

void drawRect(uint8_t col, uint16_t x,uint16_t y) {
	SDL_Rect fillRect = { x * MODIFIER, y * MODIFIER, MODIFIER, MODIFIER };
	switch (col) {
		case WHITE:
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		break;
		case LIGHT_GRAY:
		SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 0xFF);
		break;
		case DARK_GRAY:
		SDL_SetRenderDrawColor(gRenderer, 0x77, 0x77, 0x77, 0xFF);
		break;
		case BLACK:
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		break;
	}
	SDL_RenderFillRect(gRenderer, &fillRect);
}

void GFX_render(void) {
	for (uint8_t y = 0; y < LCD_HEIGHT; y++) {
		for (uint8_t x = 0; x < LCD_WIDTH; x++) {
			drawRect(gb_lcd.buffer[(y * LCD_WIDTH) + x], x, y);
		}
	}
}

void GFX_display(void) {
	SDL_RenderPresent(gRenderer);
}
