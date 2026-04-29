#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int (*helloworld)(const char*, ...);

    helloworld = &printf;

    char* printf = "printf";

    helloworld(printf);
    
    int rear = 4;
    int lenght = 3;

    rear = (++rear) % lenght;

    fprintf(stdout,"%d\n", rear);


    int height = 10;
    int height_top = 5;

    for (;height_top != height - 3; height_top++) {   
    }
    
    fprintf(stdout, "%d\n", height_top);

    return 0;
}