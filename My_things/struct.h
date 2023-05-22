#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma once
#pragma pack(push, 1)


typedef struct{
    unsigned short signature; // тип файла, должен быть равен 0x4D42
    unsigned int filesize; // размер файла в байтах
    unsigned short reserved1; // зарезервированно, должно быть равно 0
    unsigned short reserved2; // зарезервированно, должно быть равно 0
    unsigned int pixelArrOffset; // смещение данных изображения от начала файла в байтах (то есть байтики до начала пикселей)
} BitmapFileHeader; // для всех версий один

typedef struct{
    unsigned int headerSize; // размер структуры в байтах, 
    unsigned int width; // ширина изображения в пикселях
    unsigned int height; // высота изображения в пикселях
    unsigned short planes; // количество плоскостей (должно быть 1)
    unsigned short bitsPerPixel; // количество бит на пиксель (1, 4, 8, 16, 24 или 32)
    unsigned int compression; // тип сжатия (0 - без сжатия)
    unsigned int imageSize; // размер изображения в байтах (может быть 0, если без сжатия)
    unsigned int xPixelsPerMeter; // Горизонтальное разрешение в пикселях на метр
    unsigned int yPixelsPerMeter; // Вертикальное разрешение в пикселях на метр
    unsigned int colorsInColorTable; // Количество цветов в палитре (0 - все цвета)
    unsigned int importantColorCount; // Количество важных цветов (0 - все цвета важны)
} BitmapInfoHeader; // это по факту третья версия БМП, тк поля аналогичные

typedef struct tagFXPT2DOT30 {
    int fract:30;  // Дробная часть числа
    int value:2;   // Целая часть числа
} FXPT2DOT30;
typedef struct tagFXPT230 {
    int fract:30;  // Дробная часть числа
    int value:2;   // Целая часть числа
} FXPT230;

typedef struct tagCIYZ {
    FXPT230 ciexyzX;  // Координата X в цветом пространств CIEXYZ
    FXPT2DOT30 ciexyz;  // Координата Y в цветовом пространстве CIEXYZ
    FXPT2DOT30 ciexyzZ;  // Координата Z в цветовом пространстве CIEXYZ
} CIEXYZ;

typedef struct tagCIEXYZTRIPLE {
    CIEXYZ ciexyzRed;    // Координаты точки в цветовом пространстве CIEXYZ для красного цвета
    CIEXYZ ciexyzGreen;  // Координаты точки в цветовом пространстве CIEXYZ для зеленого цвета
    CIEXYZ ciexyzBlue;   // Координаты точки в цветовом пространстве CIEXYZ для синего цвета
} CIEXYZTRIPLE;

typedef struct
{
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
    // V4 structure fields
    unsigned int RedMask;
    unsigned int GreenMask;
    unsigned int BlueMask;
    unsigned int AlphaMask;
    unsigned int CSType;
    CIEXYZTRIPLE Endpoints;
    unsigned int GammaRed;
    unsigned int GammaGreen;
    unsigned int GammaBlue;
    // V5 structure fields
    unsigned int Intent;
    unsigned int ProfileData;
    unsigned int ProfileSize;
    unsigned int Reserved;
} BitmapV5Header;             // 124 bytes


typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    
} RGB;

typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
    
} RGBA;


typedef struct BMP5
{
    BitmapFileHeader bmfh; // 14 bytes
    BitmapV5Header bmih;   // 124 bytes
    RGBA** data;            // 8 bytes
} BMP5;  

typedef struct{
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    RGB** data;
} BMP;

#pragma pack(pop)


typedef struct Point{
    int x1;
    int y1;
} Point;

typedef struct Line{
    Point p1;
    Point p2;
} Line;
