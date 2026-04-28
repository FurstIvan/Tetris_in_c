#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int (*helloworld)(const char*, ...);

    helloworld = &printf;

    char* printf = "printf";

    helloworld(printf); 
}