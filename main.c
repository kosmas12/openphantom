/*
libopenphantom test program - A test program for libopenphantom
Copyright (C) 2021 Kosmas Raptis

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include "include/openphantom.h"
#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Surface *windowSurface;
OpenPhantomImage image;


void Init() {

    // In case of error, the returned pixelData is NULL
    image = OpenPhantom_LoadImage("openphantom.bmp");
    if (!image.pixelData) {
        // More info can be fetched with OpenPhantom_GetError()
        printf("Couldn't load image: %s\n", OpenPhantom_GetError());
        exit(1);
    }

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("OpenPhantom Test",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          image.width, image.height, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Couldn't create window: %s\n", SDL_GetError());
    }

    windowSurface = SDL_GetWindowSurface(window);
    if (!windowSurface) {
        printf("Couldn't create window surface: %s\n", SDL_GetError());
    }
}

void drawImage() {
    // By default, bitmaps are inverted due to how they're stored.
    OpenPhantom_InvertImageVertically(&image);

    // Directly copy the pixel data
    SDL_LockSurface(windowSurface);
    uint32_t *pixels = windowSurface->pixels;

    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            int pixelPosition = y * image.width + x;
            pixels[pixelPosition] = image.pixelData[pixelPosition];
        }
    }
    SDL_UnlockSurface(windowSurface);
    SDL_UpdateWindowSurface(window);
}

int main() {
    printf("Testing OpenPhantom...\n");

    Init();
    drawImage();

    SDL_Event event;
    int exitted = 0;

    while (!exitted) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exitted = 1;
                    break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
