#pragma once

typedef struct {
    int w, h;
    unsigned char *data;
} Image;

unsigned char clamp(int v);
unsigned char* px(Image *img, int x, int y);

