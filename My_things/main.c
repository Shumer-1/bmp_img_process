#include "struct.h"
#include "input_bmp.h"
#include "rgb_filter.h"
#include "replace_color.h"
#include "generate_bmp.h"
#include "copy_reflection.h"

int main(){
    BMP img = generateBMP(500, 500, "diff");
    saveBMP("./Images/result.bmp", img);
 
    BMP img2 = openBMP("./Images/result.bmp");
    
    //filter("B", 255, &img);

    RGB old = {255, 0, 255};
    RGB new = {0, 255, 0};
    //replace(old, new, &img2);
    Point p1 = {0, 0};
    Point p2 = {100, 100};
    Point p_ = {450, 450};
    copy(&img2, p1, p2, p_);
    saveBMP("./Images/result_2.bmp", img2);
 
    return 0;
}