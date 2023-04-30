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
    
}