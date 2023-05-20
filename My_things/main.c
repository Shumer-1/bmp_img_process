#include "struct.h"
#include "input_bmp.h"
#include "rgb_filter.h"
#include "replace_color.h"
#include "generate_bmp.h"
#include "copy_reflection.h"
#include "draw.h"
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>


void printHelp(){
    printf("So, general provisions. First you need to enter the name of the program, \nthen the short or long expressions indicated below. Commands must be typed on a line. Thank you. \nAnd all commands must have one of the main specificator: C, f, F, R\n\n");
    printf("The origin of the coordinate system is in the lower right corner. That is, the point with coordinates (0,0) is the bottom right point.\n");
    printf("\nPlease enter correct coordinates. If the coordinates are incorrect, a hint will appear on the screen\n");

    printf("The following are the flags required to specify a job. Enter them in this format ./main -h or ./main -f R/255\n");
    
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
    printf("-R or --reflect - coordinates of line points and area points in format X1/Y1/X2/Y2/X3/Y3/X4/Y4/X5/X6\n");
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
    int flag_command; // 0 - copy, 1 - reflect, 2 - replace, 3 - change
    // reflection
    Line r_l; // ось отражения
    Point r_p1; // первая точка (top left)
    Point r_p2; // вторая точка (bot right)
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
    commands->filename = NULL;
    commands->save_filename = NULL;
    commands->flag_command = 100;
    while ((tmp = getopt_long(argc, argv, short_options, long_options, &long_index)) != -1){
        switch (tmp){
            case 'h':
                printHelp();
                commands->flag_command = 4;
                break;
            case 'I':
                commands->filename = optarg;
                break;
            case 'O':
                commands->save_filename = optarg;
                break;
            case 'i':
                commands->flag_command = 5;
                break;
            case 'C':
                // строка вида: X1,Y1/X2,Y2/X3,Y3
                char *a_c = malloc(sizeof(char)*100);
                a_c = strtok(optarg, "/,");
                int coords_c[6];
                for (int i = 0; i < 6; i++){
                    if (a_c == NULL){
                        puts("Error, you may have entered too few coordinates");
                        exit(0);
                    } 
                    if (isNum(a_c)){
                        coords_c[i] = atoi(a_c);
                        a_c = strtok(NULL, "/,");
                    }
                    else{
                        puts("Error coordiantes");
                        exit(0);
                    }
                    if ((i % 2 == 0 && (coords_c[i] > file.bmih.height || coords_c[i] < 0))
                    || ((i % 2 == 1) && (coords_c[i] > file.bmih.width || coords_c[i] < 0))){
                        puts("Error coordiantes");
                        exit(0);
                    }
                }
                commands->c_p1.x1 = coords_c[0];
                commands->c_p1.y1 = coords_c[1];
                commands->c_p2.x1 = coords_c[2];
                commands->c_p2.y1 = coords_c[3];
                commands->c_p.x1 = coords_c[4];
                commands->c_p.y1 = coords_c[5];
                commands->flag_command = 0;
                free(a_c);
                break;
            case 'f':
                // строка вида: R/0 or G/0 or B/0 (or 255)
                char *a_f = (char*)malloc(sizeof(char)*100);
                a_f = strtok(optarg, "/,");
                char A;
                int B;
                A = a_f[0];
                a_f = strtok(NULL, "/");
                if (isNum(a_f)){
                    B = atoi(a_f);
                }
                else{
                    puts("Error coordiantes");
                    exit(0);
                }
                commands->cg_parametr = A;
                commands->cg_value = B;
                commands->flag_command=3;
                free(a_f);
                break;
            case 'F':
                // строка вида: R/G/B/R/G/B (of course, R, G, B are integer in range 0-255
                char *a_F = malloc(sizeof(char)*100);
                a_F = strtok(optarg, "/,");
                int digits[6];
                for (int i = 0; i < 6; i++){
                    if (a_F == NULL){
                        puts("Error, you may have entered too few coordinates");
                        exit(0);
                    } 

                    if (isNum(a_F)){
                        digits[i] = atoi(a_F);
                        if (digits[i] > 255 && digits[i] < 0){
                            puts("Error coordiantes");
                            exit(0);
                        }
                        a_F = strtok(NULL, "/,");
                    }
                    else{
                        puts("Error coordiantes");
                        exit(0);
                    }
                }
                commands->flag_command = 2;
                commands->rc_color_1.r = digits[0];
                commands->rc_color_1.g = digits[1];
                commands->rc_color_1.b = digits[2];
                commands->rc_color_2.r = digits[3];
                commands->rc_color_2.g = digits[4];
                commands->rc_color_2.b = digits[5];
                
                free(a_F);
                break;
            case 'R':
                // строка вида: X1/Y1/X2/Y2/X3/Y3/X4/Y4/
                char *a_r = malloc(sizeof(char)*100);
                a_r = strtok(optarg, "/,");
                int coords_r[8];
                for (int i = 0; i < 8; i++){
                    if (a_r == NULL){
                        puts("Error, you may have entered too few coordinates");
                        exit(0);
                    } 
                    if (isNum(a_r)){
                        coords_r[i] = atoi(a_r);
                        a_r = strtok(NULL, "/,");
                    }
                    else{
                        puts("Error coordiantes");
                        exit(0);
                    }
                    if ((i % 2 == 0 && (coords_r[i] > file.bmih.height || coords_r[i] < 0))
                    || ((i % 2 == 1) && (coords_r[i] > file.bmih.width || coords_r[i] < 0))){
                        puts("Error coordiantes");
                        exit(0);
                    }
                    
                }
                commands->r_l.p1.x1 = coords_r[0];
                commands->r_l.p1.y1 = coords_r[1];
                commands->r_l.p2.x1 = coords_r[2];
                commands->r_l.p2.y1 = coords_r[3];
                commands->r_p1.x1 = coords_r[4];
                commands->r_p1.y1 = coords_r[5];
                commands->r_p2.x1 = coords_r[6];
                commands->r_p2.y1 = coords_r[7];

                commands->flag_command = 1;
                free(a_r);
                break;
            default:
                puts("No such argument required, read usage (-H or --help)");
                exit(0);
        }
    }
}

BMP file;

int main(int argc, char** argv){
    // BMP img2 = openBMP("./Images/picture.bmp");
    usefullCommands * commands = malloc(sizeof(usefullCommands));
    func_getopt(argc, argv, commands);
    if (commands->flag_command == 4){
        puts("You chose to see explanation");
    }
    if (commands->flag_command == 5){
        puts("You chose to see information about the file");
        if (commands->filename != NULL){
            file = openBMP(commands->filename);
            printf("Filename:%s\n Files'size is %d(width) %d(height)\n The number of image pixels is %d\n", commands->filename, file.bmih.width, file.bmih.height,
            file.bmih.width*file.bmih.height);
        }
    }
    if (commands->flag_command == 3){ //change
        if (commands->cg_parametr == 'B' || commands->cg_parametr == 'R' || commands->cg_parametr == "G"){
            if (commands->cg_value == 255 || commands->cg_value == 0){
                if (commands->filename == NULL){
                    puts("Ошибка ввода");
                    exit(0);
                }
                filter(commands->cg_parametr, commands->cg_value, &file);
                if (commands->save_filename != NULL){
                    saveBMP(commands->save_filename, file);
                }
                else{
                    puts("Output filename wasn`t given");
                }

            }
            else{
                puts("Ошибка ввода");
                exit(0);
            }
        }
        else{
            puts("Ошибка ввода");
            exit(0);
        }
    }
    if (commands->flag_command == 2){//replace
        if (commands->filename != NULL){
            file = openBMP(commands->filename);  
            replace(commands->rc_color_1, commands->rc_color_2, &file);
            if (commands->save_filename){
                saveBMP(commands->save_filename, file);
            }
        }
        else{
            puts("You didn`t give me filename");
        }
    }
    if (commands->flag_command == 1){//reflect 
        //тут полная жесть, проверки на все
        if (commands->filename != NULL){
            file = openBMP(commands->filename);    
            reflection(&file, commands->r_l, commands->r_p1, commands->r_p2);
            if (commands->save_filename != NULL){
                saveBMP(commands->save_filename, file);
            }
        }
        else{
            puts("You didn`t give me filename");
        }
    }
    if (commands->flag_command == 0){//copy
        if (commands->filename){
            file = openBMP(commands->filename);
            copy(&file, commands->c_p1, commands->c_p2, commands->c_p);
            if (commands->save_filename != NULL){
                saveBMP(commands->save_filename, file);
            }
         }
         else{
            puts("You didn`t give me filename");
         }
    }
    else{
        puts("Nothing interesting");
    }
    free(commands);
    return 0;
}
