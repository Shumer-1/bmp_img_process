#include "input_bmp.h"
#include "struct.h"



void saveBMP(const char* path, BMP img){
    FILE * f = fopen(path, "wb"); // байтовая запись
    fwrite(&img.bmfh, 1, sizeof(BitmapFileHeader), f);
    fwrite(&img.bmih, 1, sizeof(BitmapInfoHeader), f);

    size_t W = img.bmih.width;
    size_t H = img.bmih.height;

    for (size_t i = 0; i < H; i++){
        fwrite(img.data[i], 1, sizeof(RGB)* W, f);
    }
    fclose(f);
}

BMP openBMP(const char* path){
    FILE* f = fopen(path, "rb");
 
    BMP img;
 
    fread(&img.bmfh, 1, sizeof(BitmapFileHeader), f);
    fread(&img.bmih, 1, sizeof(BitmapInfoHeader), f);
 
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
    
    img.data = (RGB**)malloc(sizeof(RGB*)*H);
    for (size_t i=0; i<H; i++){
        img.data[i] = (RGB *)malloc(sizeof(RGB) * W);
        fread(img.data[i], 1, sizeof(RGB) * W, f);
    }
 
    fclose(f);
 
    return img;
}