all: main.o input_bmp.o rgb_filter.o replace_color.o generate_bmp.o copy_reflection.o
	gcc main.o input_bmp.o rgb_filter.o replace_color.o generate_bmp.o  copy_reflection.o -o main

main.o: main.c struct.h input_bmp.h rgb_filter.h replace_color.h copy_reflection.h
	gcc -c main.c

input_bmp.o: input_bmp.c struct.h input_bmp.h
	gcc -c input_bmp.c

rgb_filer.o: rgb_filter.c struct.h rgb_filter.h
	gcc -c rgb_filter.c

replace_color.o: replace_color.c replace_color.h struct.h
	gcc -c replace_color.c

generate_bmp.o: struct.h generate_bmp.c generate_bmp.h
	gcc -c generate_bmp.c

copy_reflection.o: struct.h copy_reflection.c copy_reflection.h
	gcc -c copy_reflection.c

clean:
	rm -rf *.o