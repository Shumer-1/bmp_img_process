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
    else if (img.bmih.bitsPerPixel == 32){
        for (size_t i = 0; i != H; i++){
            fwrite(img.data[i], sizeof(RGBA)* W, 1, f);
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
 
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
    if (img.bmih.bitsPerPixel == 24){
        img.data = (RGB**)malloc(sizeof(RGB*)*H);
        for (size_t i = 0; i != H; i++){
            img.data[i] = (RGB *)malloc(sizeof(RGB) * W);
            fread(img.data[i], sizeof(RGB) * W, 1, f);
        }
    }
    else if (img.bmih.bitsPerPixel == 32){
        puts("Для 32-битной версии доступна только информация о файле");
        // img.data = (RGBA**)malloc(sizeof(RGBA*)*H);
        // for (size_t i = 0; i != H; i++){
        //     img.data[i] = (RGBA *)malloc(sizeof(RGBA) * W);
        //     fread(img.data[i], sizeof(RGBA) * W, 1, f);
        // }
        
    }
    fclose(f);
 
    return img;
}
