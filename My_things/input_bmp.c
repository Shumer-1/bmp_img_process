#include "input_bmp.h"
#include "struct.h"



void saveBMP(const char* path, BMP img){
    FILE * f = fopen(path, "wb"); // байтовая запись
    fwrite(&img.bmfh, sizeof(BitmapFileHeader), 1, f);
    fwrite(&img.bmih, sizeof(BitmapInfoHeader), 1, f);

    size_t W = img.bmih.width;
    size_t H = img.bmih.height;

    for (size_t i = H-1; i != 0; i--){
        fwrite(img.data[i], sizeof(RGB)* W, 1, f);
    }
    fclose(f);
}

BMP openBMP(const char* path){
    FILE* f = fopen(path, "rb");
 
    BMP img;
 
    fread(&img.bmfh, sizeof(BitmapFileHeader), 1, f);
    fread(&img.bmih, sizeof(BitmapInfoHeader), 1, f);
 
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
    
    img.data = (RGB**)malloc(sizeof(RGB*)*H);
    for (size_t i = H-1; i != -1; i--){
        img.data[i] = (RGB *)malloc(sizeof(RGB) * W);
        fread(img.data[i], sizeof(RGB) * W, 1, f);
    }
 
    fclose(f);
 
    return img;
}
