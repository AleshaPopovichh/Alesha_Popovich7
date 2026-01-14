#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bmp.h"

#pragma pack(push,1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMPFileHeader;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPInfoHeader;
#pragma pack(pop)

unsigned char clamp(int v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return (unsigned char)v;
}

unsigned char* px(Image *img, int x, int y) {
    return img->data + (y * img->w + x) * 3;
}

int load_bmp(const char *name, Image *img) {
    FILE *f = fopen(name, "rb");
    if (!f) return 1;

    BMPFileHeader fh;
    BMPInfoHeader ih;

    fread(&fh, sizeof(fh), 1, f);
    fread(&ih, sizeof(ih), 1, f);

    if (fh.bfType != 0x4D42 || ih.biBitCount != 24 || ih.biCompression != 0) {
        fclose(f);
        return 2;
    }

    img->w = ih.biWidth;
    img->h = abs(ih.biHeight);
    img->data = malloc(img->w * img->h * 3);
    if (!img->data) {
        fclose(f);
        return 3;
    }

    int row = (img->w * 3 + 3) & ~3;
    unsigned char *tmp = malloc(row);

    for (int y = img->h - 1; y >= 0; y--) {
        fread(tmp, row, 1, f);
        memcpy(img->data + y * img->w * 3, tmp, img->w * 3);
    }

    free(tmp);
    fclose(f);
    return 0;
}

int save_bmp(const char *name, Image *img) {
    FILE *f = fopen(name, "wb");
    if (!f) return 1;

    int row = (img->w * 3 + 3) & ~3;
    int size = row * img->h;

    BMPFileHeader fh = {0x4D42, 54 + size, 0, 0, 54};
    BMPInfoHeader ih = {40, img->w, img->h, 1, 24, 0, size, 0, 0, 0, 0};

    fwrite(&fh, sizeof(fh), 1, f);
    fwrite(&ih, sizeof(ih), 1, f);

    unsigned char *pad = calloc(1, row);
    for (int y = img->h - 1; y >= 0; y--) {
        memcpy(pad, img->data + y * img->w * 3, img->w * 3);
        fwrite(pad, row, 1, f);
    }

    free(pad);
    fclose(f);
    return 0;
}

