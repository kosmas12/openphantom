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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "openphantom.h"

char *OpenPhantomLastError;

OpenPhantomImage OpenPhantom_ReturnErrImage() {
    OpenPhantomImage image;
    image.pixelData = NULL;
    image.size = 0;
    image.width = 0;
    image.height = 0;
    return image;
}

void OpenPhantom_SetError(const char *error) {
    OpenPhantomLastError = error;
}

// Gets the last error reported by the library
char *OpenPhantom_GetError() {
    return OpenPhantomLastError;
}

void OpenPhantom_InvertImageVertically(OpenPhantomImage *image) {
    int i = 0;
    int numPixels = (image->width * image->height);

    while (i < image->height) {
        uint32_t *topRow = &image->pixelData[i * image->width];
        uint32_t *bottomRow = &image->pixelData[numPixels - (i * image->width) - image->width];

        for (int j = 0; j < (image->width); ++j) {
            uint32_t tempPixel = topRow[j];
            topRow[j] = bottomRow[j];
            bottomRow[j] = tempPixel;
        }

        i += 2;

    }
}

// Load bitmap image file
OpenPhantomImage OpenPhantom_LoadBMP(FILE *f) {
    OpenPhantomImage bitmap;

    // Get file size
    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);

    // Seek back to beginning
    fseek(f, 0, SEEK_SET);

    unsigned char *dataWithHeader = malloc(fileSize);

    fread(dataWithHeader, sizeof(unsigned char), fileSize, f);
    fclose(f);

    if(dataWithHeader[0x0] != 'B' || dataWithHeader[0x1] != 'M') {
        OpenPhantom_SetError("Bitmap signature missing, are you sure this is a valid bitmap file?");
        free(dataWithHeader);
        return OpenPhantom_ReturnErrImage();
    }

    int *width = (int *) &dataWithHeader[0x12];
    bitmap.width = *width;
    int *height = (int *) &dataWithHeader[0x16];
    bitmap.height = *height;
    bitmap.pixelData = (uint32_t *) &dataWithHeader[0x36];

    return bitmap;
}

OpenPhantomImage OpenPhantom_LoadImage(const char *filename) {
    FILE *f;

    f = fopen(filename, "rb");
    if (!f) {
        OpenPhantom_SetError("Couldn't open file");
        return OpenPhantom_ReturnErrImage();
    }

    const char *extension;
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) {
        printf("No file extension given.\n");
        return OpenPhantom_ReturnErrImage();
    }
    extension = dot + 1;

    if (strcmp(extension, "bmp") == 0) {
        return OpenPhantom_LoadBMP(f);
    }
    else {
        OpenPhantom_SetError("Unsupported image type provided");
        return OpenPhantom_ReturnErrImage();
    }

}
