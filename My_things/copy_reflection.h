#include "struct.h"

#pragma once

typedef struct Point{
    int x1;
    int y1;
} Point;

typedef struct Line{
    Point p1;
    Point p2;
} Line;

void copy(BMP* img, Point p1, Point p2, Point p_);