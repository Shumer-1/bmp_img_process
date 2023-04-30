#include "replace_color.h"

void replace(RGB old_color, RGB new_color, BMP* img){
    for (int i = 0; i < img->bmih.height; i++){
        for (int j = 0; j < img->bmih.width; j++){
            if (img->data[i][j].r == old_color.r && img->data[i][j].g == old_color.g && img->data[i][j].b == old_color.b){
                img->data[i][j].r = new_color.r;
                img->data[i][j].g = new_color.g;
                img->data[i][j].b = new_color.b;
            }
        }
    }
}