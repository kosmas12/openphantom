/*
libopenphantom - A simple image loading and manipulation library
Copyright (C) 2021-2022 Kosmas Raptis

This library is licensed under the MIT license. See COPYING for details
*/

#ifndef OPENPHANTOM_H
#define OPENPHANTOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    // TODO: use void* instead of uint32_t* for flexibility
    void *pixelData;
    uint16_t depth;
    int width;
    int height;
}OpenPhantomImage;

OpenPhantomImage OpenPhantom_ReturnErrImage();
void OpenPhantom_SetError(char *error);
char *OpenPhantom_GetError();
void OpenPhantom_InvertImageVertically(OpenPhantomImage *image);
OpenPhantomImage OpenPhantom_LoadBMP(FILE *f);
OpenPhantomImage OpenPhantom_LoadImage(const char *filename);

#ifdef __cplusplus
}
#endif

#endif //OPENPHANTOM_H
