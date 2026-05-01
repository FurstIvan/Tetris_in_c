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

void _foo (void) //actual function
{
    printf("foo\n\n");
    return;
}

void (*foo_v)(void); // foo_v named variable to a function that has this stly

void* foo(void); // without the brackets its a funciton declaration

typedef void (*foo_t)(void); // typedefiniton declaration of func pointers

typedef void foo_t_d(void); // now this comboo is a function declaration 
foo_t_d foo_t_d_v; 

int main(void) {

    foo_t foo_p = _foo; // here is said foo_p is going to be equal to the address of function

    foo_v = _foo; // here i set the variable to be that;

    foo_v();
    foo_p();

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