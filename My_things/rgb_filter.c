#include "rgb_filter.h"

void filter(char component, int value, BMP* img){
    if (value != 255 && value != 0){
        puts("Wrong value for component");
        return;
    }
    if (component == 'R'){
        for (int i = 0; i < img->bmih.height; i++){
            for (int j = 0; j < img->bmih.width; j++){
                if (img->data[i][j].r != value) {
                    img->data[i][j].r = value;
                }
            }
        }
    }
    else if (component == 'G'){
        for (int i = 0; i < img->bmih.height; i++){
            for (int j = 0; j < img->bmih.width; j++){
                if (img->data[i][j].g != value) {
                    img->data[i][j].g = value;
                }
            }
        }
    }
    else if (component == 'B'){
        for (int i = 0; i < img->bmih.height; i++){
            for (int j = 0; j < img->bmih.width; j++){
                if (img->data[i][j].b != value) {
                    img->data[i][j].b = value;
                }
            }
        }
    }
    else{
        puts("Wrong name of component");
    }
    return;
}