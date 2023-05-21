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
} BitmapFileHeader;

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
