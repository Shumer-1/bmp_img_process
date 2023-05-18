#include "copy_reflection.h"
#include "draw.h"

void copy(BMP* img, Point p1, Point p2, Point p_){

    int w = p2.y1 - p1.y1 + 1;
    int h = p2.x1 - p1.x1 + 1;

    int x = p1.x1;
    int i = p_.x1;
    while (1){
        if (i == img->bmih.height || i - p_.x1 > h){
            break;
        }
        int y = p1.y1+28;
        int j = p_.y1;
        while (j < img->bmih.width && j - p_.y1 <= w){
            img->data[i][j++] = img->data[x][y++];
        }
        i++;
        x++;
    } 


}

void reflection(BMP* img, Line line, Point p1, Point p2){
    // запишем уравнение прямой
    Point new_p1; // нижняя левая точка (принцип зеркала)
    Point new_p2; // правая верхняя
    if (line.p1.x1 == line.p2.x1){ // горизонтальная
        Point pf;
        pf.x1 = p2.x1 + 2* (line.p1.x1 - p2.x1);
        pf.y1 = p1.y1;
        copy(img, p1, p2, pf);
        RGB color;
        color.r = 0;
        color.b = 0;
        color.g = 0; 
        draw_line(img, line, color);
    }
    else if (line.p1.y1 == line.p2.y1){// вертикальная
        Point pf;
        pf.x1 = p1.x1;
        pf.y1 = p2.y1 + 2* (line.p1.y1 - p2.y1);
        copy(img, p1, p2, pf);
        RGB color;
        color.r = 0;
        color.b = 0;
        color.g = 0; 
        draw_line(img, line, color);
    }
    else{
        puts("Прямая не горизонтальная и не вертикальная, такую приколдесу я еще не сделал");
    }
}