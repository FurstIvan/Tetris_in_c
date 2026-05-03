#include <stdio.h>

struct point
{
    int a;
    int b;
};

void print_struct(struct point p){
    printf("%d", p.a);
    printf("%d\n", p.b);
    return;
}

int main(void) {

    print_struct((struct point){10,10});

    int* arr = (int []){1,0,1,2,3};

    struct point p = (struct point){ .a = 1, .b = 2 };

    printf("%d", *arr);

    return 0;
}