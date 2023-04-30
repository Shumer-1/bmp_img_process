#include <stdio.h>
#include <stdlib.h>
 
#pragma pack (push, 1)
typedef struct{
    unsigned short signature; // 2
    unsigned int filesize; // 4
    unsigned short reserved1; // 2      похоже это всякие размеры
    unsigned short reserved2; // 2
    unsigned int pixelArrOffset; // 4
} BitmapFileHeader;
 
typedef struct{
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int xPixelsPerMeter;
    unsigned int yPixelsPerMeter;
    unsigned int colorsInColorTable;
    unsigned int importantColorCount;
} BitmapInfoHeader; // это по факту третья версия БМП, тк поля аналогичные
 
typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
} RGB;
 
typedef struct{
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    RGB** data;
} BMP;
 
#pragma pack(pop)
 
BMP generateBMP(size_t W, size_t H){
    BMP img;
    img.bmfh.signature = 0x4d42;
    img.bmfh.filesize = W * H * sizeof(RGB) + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader); // держим в голове, что нам смешение не нужно, тк директивы прописали
    img.bmfh.reserved1 = 0;
    img.bmfh.reserved2 = 0;
    img.bmfh.pixelArrOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
 
    img.bmih.headerSize = 40;
    img.bmih.width = W;
    img.bmih.height = H;
    img.bmih.planes = 1;
    img.bmih.bitsPerPixel = sizeof(RGB) * 8; // *8 тк биты, а не байты
    img.bmih.compression = 0;
    img.bmih.imageSize = W * H * sizeof(RGB);
    img.bmih.xPixelsPerMeter = 4700; // плотность пикселей на метр (мы сюда фулХД запихали) типо
    img.bmih.yPixelsPerMeter = 4700;
    img.bmih.colorsInColorTable = sizeof(char);
    img.bmih.importantColorCount = sizeof(char);
 
 
    img.data = (RGB **)malloc(sizeof(RGB *) * H);
 
    for (size_t i=0; i<H; i++){
        img.data[i] = (RGB *)malloc(sizeof(RGB) * W); // опять не нужны финты со смещениями, тк мы договорились, что финты в памяти что-то там
        for (size_t j=0; j<W; j++){
            img.data[i][j].r = 255;
            img.data[i][j].g = 0;
            img.data[i][j].b = 0;
        }
    }
 
    return img;
}
 
void saveBMP(const char* path, BMP img){
    FILE* f = fopen(path, "wb"); // байтовая запись
    fwrite(&img.bmfh, 1, sizeof(BitmapFileHeader), f);
    fwrite(&img.bmih, 1, sizeof(BitmapInfoHeader), f);
 
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
 
    for (size_t i=0; i<H; i++){
        fwrite(img.data[i], 1, sizeof(RGB) * W, f);
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

    for (size_t i=0; i<H; i++){
        img.data[i] = (RGB *)malloc(sizeof(RGB) * W);
        fread(img.data[i], 1, sizeof(RGB) * W, f); // тут не увидел мб тут что-то неработает
    }

    fclose(f);
 
    return img;
}
 
//  и опа, курсач всё
 
// void DrawLine(BMP img, ) {}
// void DrawLine(BMP img, ) {}
// void DrawLine(BMP img, ) {}
// void DrawLine(BMP img, ) {}
// void DrawLine(BMP img, ) {}
 
int main(){
    BMP img = generateBMP(500, 200);
 
    saveBMP("/home/user1/simple_c/sems/04.апрель/27_apr/img.bmp", img);
 
    BMP img2 = openBMP("/home/user1/simple_c/sems/04.апрель/27_apr/img.bmp");
    saveBMP("/home/user1/simple_c/sems/04.апрель/27_apr/img_D.bmp", img2);
 
 
    return 0;
}