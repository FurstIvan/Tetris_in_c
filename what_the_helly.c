#include <stdio.h>
#include <stdlib.h>

typedef enum {
    RAMP0 = 100,
    RAMP1 = 95,
    RAMP2 = 85,
    RAMP3 = 75,
    RAMP4 = 55,
    RAMP5 = 45,
    RAMP6 = 40,
    RAMP7 = 30,
    RAMP8 = 20,
    RAMP9 = 15,
    END   = 10
}Gravity;

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

    Gravity grav = RAMP0;
    fprintf(stdout,"%d\n", grav);

    grav += 1;
    fprintf(stdout, "%d\n", grav);


    return 0;
}