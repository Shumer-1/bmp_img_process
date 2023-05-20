#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
 
#pragma pack (push, 1)
 
// комментарии после цифр для этих 2х структур - от Влада
typedef struct{
    unsigned short signature; // 2, тип файла, должен быть равен 0x4D42
    unsigned int filesize; // 4, размер файла в байтах
    unsigned short reserved1; // 2, зарезервированно, должно быть равно 0
    unsigned short reserved2; // 2, зарезервированно, должно быть равно 0
    unsigned int pixelArrOffset; // 4, смещение данных изображения от начала файла в байтах
} BitmapFileHeader;
 
typedef struct{
    unsigned int headerSize; // размер структуры в байтах,
    unsigned int width; // ширина изображения в пикселях
    unsigned int height; // высота изображения в пикселях
    unsigned short planes; // количество плоскостей (должно быть 1)
    unsigned short bitsPerPixel; // количество бит на пиксель (1, 4, 8, 16, 24 или 32)
    unsigned int compression; // тип сжатия (0 - без сжатия)
    unsigned int imageSize; // размер изображения в байтах (может быть 0, если без сжатия)
    unsigned int xPixelsPerMeter; // Горизонтальное разрешение в пикселях на метр
    unsigned int yPixelsPerMeter; // Вертикальное разрешение в пикселях на метр
    unsigned int colorsInColorTable; // Количество цветов в палитре (0 - все цвета)
    unsigned int importantColorCount; // Количество важных цветов (0 - все цвета важны)
} BitmapInfoHeader; // это по факту третья версия БМП, тк поля аналогичные
 
typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
} RGB;
 
typedef struct{
    BitmapFileHeader bmfh;
    BitmapInfoHeader bmih;
    RGB** data;
} BMP;
 
#pragma pack(pop)
 
int cmp_RGB(RGB color1, RGB color2){
    return color1.b == color2.b && color1.g == color2.g &&color1.r == color2.r;
}
 
void saveBMP(const char* path, BMP img){
    FILE* f = fopen(path, "wb"); // байтовая запись
    fwrite(&img.bmfh, 1, sizeof(BitmapFileHeader), f);
    fwrite(&img.bmih, 1, sizeof(BitmapInfoHeader), f);
 
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
 
    for (size_t i=0; i<H; i++){
        fwrite(img.data[i], 1, sizeof(RGB) * W, f);
    }
 
    fclose(f);
}
 
BMP openBMP(const char* path){
    FILE* f = fopen(path, "rb");
 
    BMP img;
//    if (!f){
//        return img;
//    }
 
    fread(&img.bmfh, 1, sizeof(BitmapFileHeader), f);
    fread(&img.bmih, 1, sizeof(BitmapInfoHeader), f);
 
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
 
    img.data = (RGB **)malloc(sizeof(RGB *) * H);
    for (size_t i=0; i<H; i++){
        img.data[i] = (RGB *)malloc(sizeof(RGB) * W);
        fread(img.data[i], 1, sizeof(RGB) * W, f); // тут не увидел мб тут что-то неработает
    }
 
    fclose(f);
 
    return img;
}
 
void replace_color(BMP img, RGB color_was, RGB new_color){
    size_t W = img.bmih.width;
    size_t H = img.bmih.height;
    for (size_t y=0; y<H; y++){
        for (size_t x=0; x<W; x++){
            if (cmp_RGB(color_was, img.data[y][x])){
                img.data[y][x].b = new_color.b;
                img.data[y][x].g = new_color.g;
                img.data[y][x].r = new_color.r;
            }
        }
    }
}
 
void print_bmp(BMP img){
    printf("\n\tBitmapFileHeader\nSignature: %hu\nFilesize: %d\npixelArrOffset: %d\n"
           "\tBitmapInfoHeader\nheaderSize: %d\nwidth: %d\nheight: %d\nbitsPerPixel: %d\n"
           "compression: %d\nimageSize: %d\nxPixelsPerMeter: %d\nyPixelsPerMeter: %d\n"
           "colorsInColorTable: %d\nimportantColorCount: %d\n", img.bmfh.signature,
           img.bmfh.filesize, img.bmfh.pixelArrOffset, img.bmih.headerSize,
           img.bmih.width, img.bmih.height, img.bmih.bitsPerPixel, img.bmih.compression,
           img.bmih.imageSize, img.bmih.xPixelsPerMeter, img.bmih.yPixelsPerMeter,
           img.bmih.colorsInColorTable, img.bmih.importantColorCount);
}
 
void copy_RGB(BMP dest, BMP src, unsigned int x_dest, unsigned int y_dest, unsigned int x_src, unsigned int y_src){
//    printf("%c\n", src.data[y_src][x_src].b);
    dest.data[y_dest][x_dest].b = src.data[y_src][x_src].b;
    dest.data[y_dest][x_dest].g = src.data[y_src][x_src].g;
    dest.data[y_dest][x_dest].r = src.data[y_src][x_src].r;
}
 
BMP get_piece_of_image(BMP image, unsigned int x, unsigned int y, unsigned int width, unsigned int height){
    BMP piece;
    memmove(&piece.bmfh, &image.bmfh, sizeof(image.bmfh));
    piece.bmfh.filesize = width * height * sizeof(RGB) + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
 
    memmove(&piece.bmih, &image.bmih, sizeof(image.bmih));
    piece.bmih.width = width;
    piece.bmih.height = height;
    piece.bmih.imageSize = width * height * sizeof(RGB);
 
    piece.data = (RGB **)malloc(sizeof(RGB *) * height);
    for (unsigned int i=0; i<height; i++){
        piece.data[i] = (RGB *)malloc(sizeof(RGB) * width);
        for (unsigned int j=0; j<width; j++){
            copy_RGB(piece, image, j, i, j + x, i + y);
        }
    }
 
    return piece;
}
 
BMP copy_image(BMP image){
    BMP copy;
    memmove(&copy.bmfh, &image.bmfh, sizeof(image.bmfh));
    memmove(&copy.bmih, &image.bmih, sizeof(image.bmih));
    copy.data = (RGB **)malloc(sizeof(RGB *) * image.bmih.height);
    for (int i=0; i<image.bmih.height; i++){
        copy.data[i] = (RGB *)malloc(sizeof(RGB) * image.bmih.width);
        memmove(&copy.data[i], &image.data[i], sizeof(image.data[i][0]) * image.bmih.width);
    }
    return copy;
}
 
void cut_image(BMP image, char* folder_name, unsigned int parts_x, unsigned int parts_y){
    BMP part;
    unsigned int num;
    unsigned int width = image.bmih.width / parts_x;
    unsigned int height = image.bmih.height / parts_y;
    for (unsigned int i=0; i<parts_y; i++){
        for (unsigned int j=0; j<parts_x; j++){
            part = get_piece_of_image(image, j * width, i * height, width, height);
            num = i * parts_y + j;
            char* part_name = (char *)malloc(sizeof(char) * (strlen("Part_.bmp/") + strlen(folder_name) + 3));
//            strcpy(part_name, folder_name);
//            strcat(part_name, "/");
//            strcat(part_name, "Part_");
//            sprintf(part_name, "%d", num);
//            strcat(part_name, ".bmp");
            sprintf(part_name, "%s%s%s%d%s", folder_name, "/", "Part_", num, ".bmp");
            printf("%s\n", part_name);
            saveBMP(part_name, part);
        }
    }
}
 
void print_help(FILE* stream){
    fprintf(stream, "\n\t\tGET READY TO HAVE SOME HELP:\n\n"
           "-H or --help \t\t print help\n"
           "-I or --option_code \t what to do, 1 int in range [1:4] (1, 2, 3 - tasks written below, 4 - print info about image)\n"
           "-F or --filename \t filename of image ('---.bmp') to work with\n"
           "-O or --out_filename \t filename to image ('---.bmp') to save result in\n"
           "\t\t1) REPLACE COLOR\n"
           "-W or --which_one \t which color to replace (3 int values in range [0:255] in order RGB, divided by ':' like this '4:56:234')\n"
           "-T or --to_which \t new color to replace previous (3 int values after argument in range [0:255] in order RGB, divided by ':' like this '4:56:234')\n"
           "\t\t2) FILTER COMPONENT\n"
           "-C or --color \t\t which RGB component to filter (one single character from {'R', 'G', 'B'})\n"
           "-V or --value \t\t which value to set for component (1 int value in range [0:255]\n"
           "\t\t3) DIVIDE TO PIECES\n"
           "-X or --parts_x \t how many parts by width to do (1 int value in range [2:image width // 2])\n"
           "-Y or --parts_y \t how many parts by height to do (1 int value in range [2:image height // 2])\n"
           "-P or --folder_path \t name of folder in this directory to save images in (str name of folder folder, that does exist)\n");
}
 
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
 
int isCorrectComponent(char* str, int min, int max){
    if (!isNum(str)){
        return 0;
    }
    if (atoi(str) < min || atoi(str) > max){
        return 0;
    }
    return atoi(str);
}
 
typedef struct Commands{
    int error; // 0 - нет ошибки, остальное - да
    int option_code; // 1 - replace, 2 - filter, 3 - divide, 4 - info
    char* filename;
    char* out_filename;
    // 1 replace
    int which_one_r;
    int which_one_g;
    int which_one_b;
    int to_which_r;
    int to_which_g;
    int to_which_b;
    // 2 filter
    char color;
    int value;
    // 3 divide
    int parts_x;
    int parts_y;
    char* folder_path;
} Commands;
 
 
int folder_exists(char* folder_path){                 // НЕ ТЕСТИЛ
    if (!folder_path){
        return 0;
    }
    DIR* dir = opendir(folder_path);
    if (dir){
        return 1;
    }
    return 0;
}
 
int check_create_folder(char* folder_path){                 // НЕ ТЕСТИЛ
    if (!folder_path){
        return 0;
    }
    DIR* dir = opendir(folder_path);
    if (dir){
        return 1;
    }
    mkdir(folder_path, 0777);
    return 1;
}
 
int bmp_exists(char* filename){                 // НЕ ТЕСТИЛ
    if (!filename){
        return 0;
    }
    return access(filename, F_OK) == 0;
}
 
void do_getopt(int argc, char* argv[], Commands* coms) {
    char *short_options = "HI:F:O:W:T:C:V:X:Y:P:";
    struct option long_options[] = {
            {"help",         no_argument,       0, 'H'},
            {"option_code",  no_argument,       0, 'I'},
            {"filename",     required_argument, 0, 'F'},
            {"out_filename", required_argument, 0, 'O'},
            {"which_one",    required_argument, 0, 'W'},
            {"to_which",     required_argument, 0, 'T'},
            {"color",        required_argument, 0, 'C'},
            {"value",        required_argument, 0, 'V'},
            {"parts_x",      required_argument, 0, 'X'},
            {"parts_y",      required_argument, 0, 'Y'},
            {"folder_path",  required_argument, 0, 'P'},
    };
    int long_index, com;
    char *number;
    while ((com = getopt_long(argc, argv, short_options, long_options, &long_index)) != -1) {
        switch (com) {
            case 'H': // help
                print_help(stdout);
                break;
            case 'I': // option_code
                coms->option_code = isCorrectComponent(optarg, 1, 4);
                if (!coms->option_code) {
                    fprintf(stderr, "Value after -I (--option_code) should be int in range [1:4]!\n");
                    exit(0);
                }
                printf("option_code:\t[%d]\n", coms->option_code);
                break;
            case 'F': // filename
                coms->filename = optarg;
                printf("filename:\t[%s]\n", optarg);
                break;
            case 'O': // out_filename
                coms->out_filename = optarg;
                printf("out_filename:\t[%s]\n", optarg);
                break; //                   1) REPLACE COLOR
            case 'W': // which_one
                number = strtok(optarg, ":");
                coms->which_one_r = isCorrectComponent(number, 0, 255);
                number = strtok(NULL, ":");
                coms->which_one_g = isCorrectComponent(number, 0, 255);
                number = strtok(NULL, ":");
                coms->which_one_b = isCorrectComponent(number, 0, 255);
                if (!coms->which_one_r || !coms->which_one_g || !coms->which_one_b) {
                    fprintf(stderr,
                            "Values after -W (--which_one) should be 3 ints, divided by ':', in range [0:255]!\n");
                    exit(0);
                }
                printf("which_one:\t[%d] [%d] [%d]\n", coms->which_one_r, coms->which_one_g, coms->which_one_b);
                break;
            case 'T': // to_which
                number = strtok(optarg, ":");
                coms->to_which_r = isCorrectComponent(number, 0, 255);
                number = strtok(NULL, ":");
                coms->to_which_g = isCorrectComponent(number, 0, 255);
                number = strtok(NULL, ":");
                coms->to_which_b = isCorrectComponent(number, 0, 255);
                if (!coms->to_which_r || !coms->to_which_g || !coms->to_which_b) {
                    fprintf(stderr,
                            "Values after -T (--to_which) should be 3 ints, divided by ':', in range [0:255]!\n");
                    exit(0);
                }
                printf("to_which:\t[%d] [%d] [%d]\n", coms->to_which_r, coms->to_which_g, coms->to_which_b);
                break; //                   2) FILTER COMPONENT
            case 'C': // color
                if (!(strcmp(optarg, "R") == 0 || strcmp(optarg, "G") == 0 || strcmp(optarg, "B") == 0)) {
                    fprintf(stderr, "Value after -C (--color) should be one single character from {'R', 'G', 'B'}!\n");
                    exit(0);
                }
                coms->color = optarg[0];
                printf("color:\t[%C]\n", optarg[0]);
                break;
            case 'V': // value
                coms->value = isCorrectComponent(optarg, 0, 255);
                if (!coms->value) {
                    fprintf(stderr, "Value after -V (--value) should be int in range [0:255]!\n");
                    exit(0);
                }
                printf("value:\t[%d]\n", coms->value);
                break; //                   3) DIVIDE TO PIECES
            case 'X': // parts_x
                coms->parts_x = isCorrectComponent(optarg, 2, INT_MAX);
                if (!coms->parts_x) {
                    fprintf(stderr, "Value after -X (--parts_x) should be int >1 and obviously <%d (INT_MAX)!\n",
                            INT_MAX);
                    exit(0);
                }
                printf("parts_x:\t[%d]\n", coms->parts_x);
                break;
            case 'Y': // parts_y
                coms->parts_y = isCorrectComponent(optarg, 2, INT_MAX);
                if (!coms->parts_y) {
                    fprintf(stderr, "Value after -Y (--parts_y) should be int >1 and obviously <%d (INT_MAX)!\n",
                            INT_MAX);
                    exit(0);
                }
                printf("parts_y:\t[%d]\n", coms->parts_y);
                break;
            case 'P': // folder_path
//                if (!opendir(optarg)) {
//                    fprintf(stderr,
//                            "Str after -P (--folder_path) should be global path to folder, that exists!\n");
////                    exit(0);
//                    continue;
//                }
                if (!optarg){
                    fprintf(stderr, "There must be str after -P!\n");
                    continue;
                }
                coms->folder_path = optarg;
                printf("folder_path:\t[%s]\n", optarg);
                break;
            default:
                coms->error = 1;
//                fprintf(stderr, "No such argument required [%c], read usage (-H or --help)\n", com);
        }
    }
}
 
void task_1(Commands coms){ // replace
    BMP img = openBMP(coms.filename);
    int flag = 0;
    RGB color = {coms.which_one_r, coms.which_one_g, coms.which_one_b};
    if(strlen(coms.out_filename) > 0 && strstr(coms.out_filename, ".bmp\0")){ //            возможно без \0
        img = copy_image(img);
        flag = 1;
    }
    for (unsigned int i=0; i<img.bmih.height; i++) {
        for (unsigned int j = 0; j < img.bmih.width; j++) {
            if (cmp_RGB(img.data[i][j], color)){
                img.data[i][j].b = coms.to_which_b;
                img.data[i][j].r = coms.to_which_r;
                img.data[i][j].g = coms.to_which_g;
            }
        }
    }
    if (flag){
        saveBMP(coms.out_filename, img);
    }
    else{
        saveBMP(coms.filename, img);
    }
}
 
void task_2(Commands coms){ // filter
    BMP img = openBMP(coms.filename);
    int flag = 0;
    if(strlen(coms.out_filename) > 0 && strstr(coms.out_filename, ".bmp\0")){ //            возможно без \0
        img = copy_image(img);
        flag = 1;
    }
    if (coms.color == 'G'){
        for (unsigned int i=0; i<img.bmih.height; i++) {
            for (unsigned int j = 0; j < img.bmih.width; j++) {
                img.data[i][j].g = coms.value;
            }
        }
    }
    else if (coms.color == 'R'){
        for (unsigned int i=0; i<img.bmih.height; i++) {
            for (unsigned int j = 0; j < img.bmih.width; j++) {
                img.data[i][j].r = coms.value;
            }
        }
    }
    else if (coms.color == 'B'){
        for (unsigned int i=0; i<img.bmih.height; i++) {
            for (unsigned int j = 0; j < img.bmih.width; j++) {
                img.data[i][j].b = coms.value;
            }
        }
    }
    else{
        printf("task_2: No such component!\n");
    }
    if (flag){
        saveBMP(coms.out_filename, img);
    }
    else{
        saveBMP(coms.filename, img);
    }
 
}
 
void task_3(Commands coms){ // divide
    BMP img = openBMP(coms.filename);
    cut_image(img, coms.folder_path, coms.parts_x, coms.parts_y);
}
 
void task_4(Commands coms){
    BMP img = openBMP(coms.filename);
    print_bmp(img);
}
 
void do_tasks(Commands coms){
    BMP img;
    if(!bmp_exists(coms.filename)){
        printf("File doesn't exist!\n");
        return;
    }
    else if (!(strlen(coms.filename) > 0 && strstr(coms.filename, ".bmp\0"))){
        printf("File isn't BMP image!\n");
        return;
    }
    else{
        img = openBMP(coms.filename);
    }
    switch (coms.option_code) {
        case 1: // replace
            if (coms.which_one_r != -1 || coms.which_one_g != -1 || coms.which_one_b != -1 ||
                    coms.to_which_r != -1 || coms.to_which_g != -1 || coms.to_which_b != -1){ // всё верно
                task_1(coms);
                break;
            }
            else{
                printf("Replace has incorrect args, so try again\n");
            }
        case 2: // filter
            if (coms.value >= 0 && coms.value <= 255 &&
            (coms.color == 'G' || coms.color == 'B' || coms.color == 'R')){
                task_2(coms);
                break;
            }
            else{
                printf("Filter has incorrect args, so try again\n");
            }
        case 3: // divide
//            printf("%s\n", coms.folder_path);
//            printf("%d\n", check_create_folder(coms.folder_path));
            if (coms.parts_x >=2 && coms.parts_x <= img.bmih.width / 2 &&
                coms.parts_y >=2 && coms.parts_y <= img.bmih.height / 2 &&
                check_create_folder(coms.folder_path)){
                task_3(coms);
                break;
            }
            else{
                printf("Divide has incorrect args, so try again\n");
            }
            break;
        case 4: // info
            task_4(coms);
            break;
        default:
            if (coms.error != 0){
                printf("An error occurred, use program correctly (-h for help)\n");
            }
            else{
                printf("No such option code, try different (-h for help)\n");
            }
            return;
            break;
    }
}
 
int main(int argc, char* argv[]){
    Commands coms = {0, -1, "", "", -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, NULL};
    do_getopt(argc, argv, &coms);
//    coms.option_code = 3;
//    coms.filename = "main.c";
//    coms.parts_y = 2;
//    coms.parts_x = 2;
//    coms.folder_path = "pieces";
    do_tasks(coms);
//    BMP img = openBMP("8.bmp");
//    BMP img2 = copy_image(img);
//    saveBMP("82.bmp", img2);
    return 0;
}