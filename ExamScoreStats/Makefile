TARGET = grades extra

all: $(TARGET)

grades: grades.c readfile.o
	gcc -g -o grades grades.c readfile.o -lm

readfile.o: readfile.c readfile.h
	gcc -g -c readfile.c

extra: extra.c
	gcc -g -Wall -o extra extra.c readfile.c -lm
	
clean:
	$(RM) $(TARGET)  *.o


