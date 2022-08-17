/*
libopenphantom - A simple image loading and manipulation library
Copyright (C) 2021-2022 Kosmas Raptis

This library is licensed under the MIT license. See COPYING for details
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "openphantom.h"

char *OpenPhantomLastError;

OpenPhantomImage OpenPhantom_ReturnErrImage() {
    OpenPhantomImage image;
    image.pixelData = NULL;
    image.width = 0;
    image.height = 0;
    return image;
}

void OpenPhantom_SetError(char *error) {
    OpenPhantomLastError = error;
}

// Gets the last error reported by the library
char *OpenPhantom_GetError() {
    return OpenPhantomLastError;
}

void OpenPhantom_InvertImageVertically(OpenPhantomImage *image) {
    int i = 0;
    int numPixels = (image->width * image->height);
    int bytesPerPixel = image->depth / 8;
    if (image->depth < 8) {
        bytesPerPixel = 1;
    }

    while (i < image->height) {
        uint8_t *topRowBytes = &image->pixelData[i * image->width * bytesPerPixel];
        uint8_t *bottomRowBytes = &image->pixelData[(numPixels - (i * image->width) - image->width) * bytesPerPixel];

        for (int j = 0; j < (image->width) * bytesPerPixel; ++j) {
            uint8_t tempByte = topRowBytes[j];
            topRowBytes[j] = bottomRowBytes[j];
            bottomRowBytes[j] = tempByte;
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

    memcpy(&bitmap.width, &dataWithHeader[0x12], sizeof(int));
    memcpy(&bitmap.height, &dataWithHeader[0x16], sizeof(int));
    memcpy(&bitmap.depth, &dataWithHeader[0x1C], sizeof(uint16_t));
    int offsetToBMPData = dataWithHeader[0xA];
    int BMPDataSize = (bitmap.width * bitmap.height * (bitmap.depth/8));
    bitmap.pixelData = malloc(BMPDataSize);
    memcpy(bitmap.pixelData, &dataWithHeader[offsetToBMPData], BMPDataSize);

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

void OpenPhantom_GetImageAs16bpp(OpenPhantomImage image) {

}

