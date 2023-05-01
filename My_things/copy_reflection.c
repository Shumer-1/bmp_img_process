#include "copy_reflection.h"


void copy(BMP* img, Point p1, Point p2, Point p_){
    
    int i = p_.x1;
    int i_ = p1.x1;
    int w = p2.y1 - p1.y1 + 1;
    int h = p2.x1 - p1.x1 + 1;

    while(i < img->bmih.height){
        int j = p_.y1;
        int j_ = p1.y1;
        while (j < img->bmih.width){
            img->data[i][j++] = img->data[i_][j_++];
            if (j - p_.y1 > w){
                break;
            }
        }
        i++;
        i_++;
        if (i - p_.x1 > h){
            break;
        }
    }
}

void reflection(BMP* img, Line line, Point p1, Point p2){
    // запишем уравнение прямой
    Point new_p1; // нижняя левая точка (принцип зеркала)
    Point new_p2; // правая верхняя
    if (line.p1.x1 == line.p2.x1){
        new_p1.y1 = (line.p1.y1 - p1.y1) + line.p1.y1; // новая y это разность и еще точка отражения
        new_p1.x1 = p1.x1;
        new_p2.y1 = (line.p2.y1 - p2.y1) + line.p2.y1;
        new_p2.x1 = p2.x1;
        Point usefull;
        usefull.x1 = new_p1.x1;
        usefull.y1 = new_p2.y1;
        copy(img, p1, p2, usefull);
    }
    else if (line.p1.y1 == line.p2.y1){
        new_p1.x1 = (line.p1.x1 - p1.x1) + line.p1.x1;
        new_p1.y1 = p1.y1;
        new_p2.x1 = (line.p2.x1 - p2.x1) + line.p2.x1;
        new_p2.y1 = p2.y1;
        Point usefull;
        usefull.x1 = new_p2.x1;
        usefull.y1 = new_p1.y1;
        copy(img, p1, p2, usefull);
    }
    else{
        puts("Прямая не горизонтальная и не вертикальная, такую приколдесу я еще не сделал");
    }
}