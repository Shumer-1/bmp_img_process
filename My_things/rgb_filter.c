#include "rgb_filter.h"

void filter(const char *component, int value, BMP* img){
    if (value != 255 && value != 0){
        puts("Wrong value for component");
        return;
    }
    // здесь должна быть проверка, что компонент норм и value тоже
    if (strcmp(component, "R") == 0){
        for (int i = 0; i < img->bmih.height; i++){
            for (int j = 0; j < img->bmih.width; j++){
                img->data[i][j].r = value;
            }
        }
    }
    else if (strcmp(component, "G") == 0){
        for (int i = 0; i < img->bmih.height; i++){
            for (int j = 0; j < img->bmih.width; j++){
                img->data[i][j].g = value;
            }
        }
    }
    else if (strcmp(component, "B") == 0){
        for (int i = 0; i < img->bmih.height; i++){
            for (int j = 0; j < img->bmih.width; j++){
                img->data[i][j].b = value;
            }
        }
    }
    else{
        puts("Wrong name of component");
    }
    return;
}