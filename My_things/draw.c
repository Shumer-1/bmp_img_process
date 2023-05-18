#include "draw.h"


void draw_line(BMP* img, Line line, RGB color){
    if (line.p1.x1 == line.p2.x1){ // горизонтальный
        for (int i = line.p1.y1; i <= line.p2.y1; i++){
            img->data[line.p1.x1][i].r = color.r;
            img->data[line.p1.x1][i].g = color.g;
            img->data[line.p1.x1][i].b = color.b;
        }
    }
    else if (line.p1.y1 == line.p2.y1){ // горизонтальный
        for (int i = line.p1.x1; i <= line.p2.x1; i++){
            img->data[i][line.p1.y1].r = color.r;
            img->data[i][line.p1.y1].g = color.g;
            img->data[i][line.p1.y1].b = color.b;
        }
    }
}