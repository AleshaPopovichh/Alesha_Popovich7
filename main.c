#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "filters.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s in.bmp out.bmp\n", argv[0]);
        return 0;
    }

    Image img;
    if (load_bmp(argv[1], &img)) {
        printf("ERROR: unsupported BMP format\n");
        return 1;
    }

    save_bmp(argv[2], &img);
    free(img.data);
    return 0;
}

