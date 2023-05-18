#include "struct.h"
#include "input_bmp.h"
#include "rgb_filter.h"
#include "replace_color.h"
#include "generate_bmp.h"
#include "copy_reflection.h"
#include "draw.h"
#include <getopt.h>
#include <unistd.h>


void printHelp(){
    printf("So, general provisions. First you need to enter the name of the program, \nthen the short or long expressions indicated below. Commands must be typed on a line. Thank you. \nAnd all commands must have one of the main specificator: C, f, F, R\n\n");

    printf("-h or --help - to read this message)\n");
    printf("-I or --input - to open file\n");
    printf("-S or --save - to save file\n");
    printf("-i or --info - to see general information about picture \n");

    printf("\n\tTo copy a area:\n\n");
    printf("-C or --copy - coordinates of points in format X1,Y1/X2,Y2/X3,Y3\n");
    
    printf("\n\tTo change one color parameter to 255 or 0:\n\n");
    printf("-f or --change - color and value of it in format R/0 or G/0 or B/0 (or 255)\n");

    printf("\n\tTo replace one color to another:\n\n");
    printf("-F or --replace - old and new colors in format R/G/B/R/G/B (of course, R, G, B are integer in range 0-255\n");

    printf("\n\tTo reflect a given area:\n\n");
    printf("-R or --reflect - coordinates of line points and area points in format /X1/Y1/X2/Y2/X3/Y3/X4/Y4/X5/X6/\n");
};

int isNum(char* str){
    if (!str){
        return 0;
    }
    for (int i=0; i < strlen(str); i++){
        if (!isdigit(str[i])){
            return 0;
        }
    }
    return 1;
}

typedef struct{
    int flag_copy;
    int flag_reflection;
    int flag_filter;
    int flag_replace_color;
} Info;

typedef struct{
    int err; // нормально - 0, ошибка - 1
    char* filename;
    char* save_filename;
    // reflection
    Line r_l; // ось отражения
    Point r_p1; // первая точка (top left)
    Point r_p2; // вторая точка (bot right)
    Point r_p; // новая точка (new top left)
    // copy
    Point c_p1; // первая точка старой области (old top left)
    Point c_p2; // вторая точка старой области (old bot right)
    Point c_p; // точка новой области (new top left)
    // replace
    RGB rc_color_1; // старый цвет
    RGB rc_color_2; // новый цвет
    // change parametr
    char cg_parametr; // буква параметра
    int cg_value; // 255 или 0

}usefullCommands;


Info func_getopt(int argc, char ** argv, usefullCommands* commands){
    // суть: передаем агрументы, первый сам ./main
    char short_options[] = "hI:iC:f:F:R:O:";
        struct option long_options[]={
        {"help", no_argument, 0, 'h'},
        {"input", required_argument, 0, 'I'},
        {"save", required_argument, 0, 'O'},
        {"info", no_argument, 0, 'i'},
        {"copy", required_argument, 0, 'C'},
        {"change", required_argument, 0, 'f'},
        {"replace", required_argument, 0, 'F'},
        {"reflect", required_argument, 0, 'R'},
    };

    int long_index, tmp;
    char *number;
    while ((tmp = getopt_long(argc, argv, short_options, long_options, &long_index)) != -1){
        switch (tmp){
            case 'h':
                printHelp();
                break;
            case 'I':
                commands->filename = optarg;
                break;
            case 'O':
                commands->save_filename = optarg;
                break;
            case 'i':
                printf("Что-то интересненькое");
                break;
            case 'C':
                char *a;
                a = strtok(optarg, '/');
                int i = 0;
                while (a != NULL){

                }
                break;
            case 'f':
                break;
            case 'F':
                break;
            case 'R':
                break;
            default:
                fprintf("No such argument required [%c], read usage (-H or --help)\n");;
        }
    }
}


int main(int argc, char** argv){
    BMP img2 = openBMP("./Images/picture.bmp");
    printHelp();
    printf("%d\n", argc);
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