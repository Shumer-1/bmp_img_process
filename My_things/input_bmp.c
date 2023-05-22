#include "input_bmp.h"
#include "struct.h"



void saveBMP(const char* path, BMP img){
    FILE * f = fopen(path, "wb"); // байтовая запись
    fwrite(&img.bmfh, sizeof(BitmapFileHeader), 1, f);
    fwrite(&img.bmih, sizeof(BitmapInfoHeader), 1, f);

    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
    if (img.bmih.bitsPerPixel == 24){
        for (size_t i = 0; i != H; i++){
            fwrite(img.data[i], sizeof(RGB)* W, 1, f);
        }
        fclose(f);
    }

    
}

BMP openBMP(const char* path){
    FILE* f = fopen(path, "rb");
    if (f == NULL){
        puts("The file was not found");
        exit(0);
    }
 
    BMP img;
 
    fread(&img.bmfh, sizeof(BitmapFileHeader), 1, f);
    fread(&img.bmih, sizeof(BitmapInfoHeader), 1, f);

    if (img.bmih.bitsPerPixel == 32){
        puts("32 бита");
        exit(0);
    }
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
    img.data = (RGB**)malloc(sizeof(RGB*)*H);
    for (size_t i = 0; i != H; i++){
        img.data[i] = (RGB *)malloc(sizeof(RGB) * W);
        fread(img.data[i], sizeof(RGB) * W, 1, f);
    }
    fclose(f);
 
    return img;
}



void saveBMP5(const char* path, BMP5 img){
    FILE* f = fopen(path, "wb");

    fwrite(&img.bmfh, sizeof(BitmapFileHeader),1, f);
    fwrite(&img.bmih, sizeof(BitmapV5Header), 1, f);
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
    for (size_t i = 0; i < H; i++){
        fwrite(img.data[i], 1, sizeof(RGBA) * W, f);
    }

    fclose(f);
}

BMP5 openBMP5(const char* path){
    FILE* f = fopen(path, "rb");

    if (f == NULL){
        puts("The file was not found");
        exit(0);
    }
    BMP5 img;

    fread(&img.bmfh, sizeof(BitmapFileHeader), 1, f);
    fread(&img.bmih, sizeof(BitmapV5Header), 1, f);
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
    img.data = (RGBA**) malloc(sizeof(RGBA*) * H);
    for (size_t i = 0; i < H; i++){
        img.data[i] = (RGBA*) malloc(sizeof(RGBA) * W);
        fread(img.data[i], 1, sizeof(RGBA) * W, f);
    }

    fclose(f);
    return(img);
}
