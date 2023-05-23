#include "copy_reflection.h"
#include "draw.h"

#define BIAS 28 //смещение изображения

void copy(BMP* img, Point p1, Point p2, Point p_){
    int w = p2.y1 - p1.y1 + 1;
    int h = p2.x1 - p1.x1 + 1;
    RGB ** new = malloc(sizeof(RGB*)*img->bmih.height);
    for (int i = 0; i < img->bmih.height; i++){
        new[i] = malloc(sizeof(RGB)*img->bmih.width);
        for (int j = 0; j < img->bmih.width; j++){
            new[i][j] = img->data[i][j];
        }
    }
    p1.y1 = p1.y1+BIAS;
    p_.y1 = p_.y1+BIAS;

    int x = p1.x1;
    int i = p_.x1;
    while (1){
        if (i == img->bmih.height || i - p_.x1 > h){
            break;
        }
        int y = p1.y1;
        int j = p_.y1;
        while (j < img->bmih.width && j - p_.y1 <= w){
            img->data[i][j++] = new[x][y++];
        }
        i++;
        x++;
    }
    for (int i = 0; i < img->bmih.height; i++){
            free(new[i]);
    }
    free(new);
}


// void reflection_but_not_reflection(BMP* img, Line line, Point p1, Point p2){
//     // запишем уравнение прямой
//     line.p1.y1+=BIAS;
//     line.p2.y1+=BIAS;
//     p1.y1+=BIAS;
//     p2.y1+=BIAS;


//     Point new_p1; // нижняя левая точка (принцип зеркала)
//     Point new_p2; // правая верхняя
//     if (line.p1.x1 == line.p2.x1){ // горизонтальная
//         Point pf;
//         pf.x1 = p2.x1 + 2 * (line.p1.x1 - p2.x1); 
//         pf.y1 = p1.y1;
//         copy(img, p1, p2, pf);
//         RGB color;
//         color.r = 0;
//         color.b = 0;
//         color.g = 0; 
//         draw_line(img, line, color);
//     }
//     else if (line.p1.y1 == line.p2.y1){// вертикальная
//         Point pf;
//         pf.x1 = p1.x1;
//         pf.y1 = p2.y1 + 2* (line.p1.y1 - p2.y1);
//         copy(img, p1, p2, pf);
//         RGB color;
//         color.r = 0;
//         color.b = 0;
//         color.g = 0; 
//         draw_line(img, line, color);
//     }
//     else{
//         puts("Прямая не горизонтальная и не вертикальная, такую приколдесу я еще не сделал");
//     }
// }



void reflection(BMP* img, Line l, Point p1, Point p2){
    Point mid;
    p1.y1+=BIAS;
    p2.y1+=BIAS;
    l.p1.y1+=BIAS;
    l.p2.y1+=BIAS;
    mid.x1 = (l.p1.y1 - p1.y1);
    mid.y1 = (l.p2.x1 - p1.x1);
    RGB color;
    color.r = 0;
    color.b = 0;
    color.g = 0; 
    draw_line(img, l, color);
    if (l.p1.x1 == l.p2.x1){ //горизонтальная
        int j_refl;
        int i_refl;
        for (int i = p1.y1; i <= p2.y1; i++){
            for (int j = p1.x1;j <= p2.x1; j++){
                mid.x1 = l.p1.x1 - i; 
                i_refl = 2 * mid.x1 + i;
                j_refl = j;
                RGB pixel = img->data[i][j];
                img->data[i_refl][j_refl] = pixel;
            } 
        }
    }
    if (l.p1.y1 == l.p2.y1){
        int j_refl;
        int i_refl;
        for (int i = p1.y1; i <= p2.y1; i++){
            for (int j = p1.x1;j <= p2.x1; j++){
                mid.y1 = l.p1.y1 - j;
                i_refl = i;
                j_refl = 2 * mid.y1 + j;
                RGB pixel = img->data[i][j];
                img->data[i_refl][j_refl] = pixel;
            } 
        }
    }
}