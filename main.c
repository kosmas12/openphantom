/*
libopenphantom test program - A test program for libopenphantom
Copyright (C) 2021-2022 Kosmas Raptis

This test program is licensed under the MIT license. See COPYING for details
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

    SDL_PixelFormatEnum format;

    switch (image.depth) {
        // 32-bit color palette used for anything else (not supported yet by OpenPhantom)
        case 1:
        case 2:
        case 4:
        case 8:
        case 32:
            format = SDL_PIXELFORMAT_ARGB8888;
            break;
        case 16:
            format = SDL_PIXELFORMAT_RGB565;
            break;
        case 24:
            format = SDL_PIXELFORMAT_RGB24;
            break;
    }

    SDL_Surface *imageSurface = SDL_ConvertSurfaceFormat(windowSurface, format, 0);

    // Directly copy the pixel data
    SDL_LockSurface(imageSurface);
    uint8_t *pixels = imageSurface->pixels;

    int bytesPerPixel = image.depth / 8;

    for (int i = 0; i < image.width * image.height * bytesPerPixel; ++i) {
        pixels[i] = ((uint8_t *) image.pixelData)[i];
    }

    SDL_UnlockSurface(imageSurface);
    SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
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
