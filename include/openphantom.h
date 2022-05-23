/*
libopenphantom - A simple image loading and manipulation library
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
