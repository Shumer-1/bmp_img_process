#include "struct.h"
#include "input_bmp.h"
#include "rgb_filter.h"
#include "replace_color.h"
#include "generate_bmp.h"
#include "copy_reflection.h"
#include "draw.h"
#include <getopt.h>

void printHelp(){
    printf("getopt example\n");
    printf("-f <value> - final value\n");
    printf("-r - reverse\n");
    printf("-v --verbose - verbose\n");
    printf("-h -? - help\n");
}

int main(){

    BMP img2 = openBMP("./Images/picture.bmp");
    //replace(old, new, &img2);
    Point p1 = {0, 100};
    Point p2 = {100, 400};
    Point p_1 = {0, 500};
    Point p_2 = {400, 500};
    Point p_ = {500, 700};
    Line l = {p_1, p_2};
    copy(&img2, p1, p2, p_);
    //reflection(&img2, l, p1, p2);
    printf("%d %d\n", img2.bmih.width, img2.bmih.height);
    //printf("%d\n", img2.bmfh.pixelArrOffset);
    saveBMP("./Images/result_2.bmp", img2);
 
    for (int i = 0; i < img2.bmih.height; i++){
        free(img2.data[i]);
    }
    free(img2.data);
    return 0;
}