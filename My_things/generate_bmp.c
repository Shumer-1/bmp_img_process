#include "generate_bmp.h"

void red_square(BMP* img, int H, int W){
    img->data = (RGB**)malloc(sizeof(RGB*) * H);
    for (int i = 0; i < H; i++){
        img->data[i] = (RGB*)malloc(sizeof(RGB)*W);
        for (int j = 0; j < W; j++){
            img->data[i][j].r = 255;
            img->data[i][j].g = 0;
            img->data[i][j].b = 0;
        }
    }
}

void blue_square(BMP* img, int H, int W){
    img->data = (RGB**)malloc(sizeof(RGB*) * H);
    for (int i = 0; i < H; i++){
        img->data[i] = (RGB*)malloc(sizeof(RGB)*W);
        for (int j = 0; j < W; j++){
            img->data[i][j].r = 0;
            img->data[i][j].g = 0;
            img->data[i][j].b = 255;
        }
    }
}

void green_square(BMP* img, int H, int W){
    img->data = (RGB**)malloc(sizeof(RGB*) * H);
    for (int i = 0; i < H; i++){
        img->data[i] = (RGB*)malloc(sizeof(RGB)*W);
        for (int j = 0; j < W; j++){
            img->data[i][j].r = 0;
            img->data[i][j].g = 255;
            img->data[i][j].b = 0;
        }
    }
}

void d_square(BMP* img, int H, int W){
    img->data = (RGB**)malloc(sizeof(RGB*) * H);
    for (int i = 0; i < H; i++){
        img->data[i] = (RGB*)malloc(sizeof(RGB)*W);
        for (int j = 0; j < W/2; j++){
            img->data[i][j].r = 0;
            img->data[i][j].g = 255;
            img->data[i][j].b = 0;
        }
    }
    for (int i = 0; i < H; i++){
        img->data[i] = (RGB*)malloc(sizeof(RGB)*W);
        for (int j = W/2; j < W; j++){
            img->data[i][j].r = 0;
            img->data[i][j].g = 255;
            img->data[i][j].b = 0;
        }
    }
}


void black_square(BMP* img, int H, int W){
    img->data = (RGB**)malloc(sizeof(RGB*) * H);
    for (int i = 0; i < H; i++){
        img->data[i] = (RGB*)malloc(sizeof(RGB)*W);
        for (int j = 0; j < W; j++){
            img->data[i][j].r = 0;
            img->data[i][j].g = 0;
            img->data[i][j].b = 0;
        }
    }
}


BMP generateBMP(size_t W, size_t H, const char * color){
    BMP img;
    img.bmfh.signature = 0x4d42;
    img.bmfh.filesize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + W*H*sizeof(RGB);
    img.bmfh.reserved1 = 0;
    img.bmfh.reserved2 = 0;
    img.bmfh.pixelArrOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    img.bmih.headerSize = 40; //без понятия, но так сделал Государкин
    img.bmih.width = W;
    img.bmih.height = H;
    img.bmih.planes = 1;
    img.bmih.bitsPerPixel = sizeof(RGB) * 8;
    img.bmih.compression = 0;
    img.bmih.imageSize = W*H*sizeof(RGB);
    img.bmih.xPixelsPerMeter = 4700;
    img.bmih.yPixelsPerMeter = 4700;
    img.bmih.colorsInColorTable = sizeof(char); // столько мы можем себе позволить
    img.bmih.importantColorCount = sizeof(char); // прикол


    if (strcmp(color, "red") == 0){
        red_square(&img, H, W);
    }
    else if (strcmp(color, "green") == 0){
        green_square(&img, H, W);
    }
    else if (strcmp(color, "blue") == 0){
        blue_square(&img, H, W);
    }
    else if (strcmp(color, "diff") == 0){
        d_square(&img, H, W);
    }
    else{
        puts("Black square");
        black_square(&img, H, W);
    }
    return img;
}