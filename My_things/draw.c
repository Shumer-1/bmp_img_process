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


void alg_draw(BMP*img, Point p1, Point p2){
    int x1 = p2.x1;
    int y1 = p2.y1;
    int x0 = p1.x1;
    int y0 = p1.y1;

    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    int error = 0;
    int deltaerr = (deltay + 1);
    int y = y0;
    int diry = y1 - y0;
    if (diry > 0) 
        diry = 1;
    if (diry < 0)
        diry = -1;
    for (int x = x0; x < x1; x++){
        RGB color = {255, 0, 0};
        img->data[x][y] = color;
        error = error + deltaerr;
        if (error >= (deltax + 1)){
            y = y + diry;
            error = error - (deltax + 1);
        }
    }
}