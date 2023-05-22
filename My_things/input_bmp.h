#pragma once
#include "struct.h"

BMP openBMP(const char* path);
void saveBMP(const char* path, BMP img);
void saveBMP5(const char* path, BMP5 img);
BMP5 openBMP5(const char* path);