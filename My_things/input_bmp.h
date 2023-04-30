#pragma once
#include "struct.h"

BMP openBMP(const char* path);
void saveBMP(const char* path, BMP img);