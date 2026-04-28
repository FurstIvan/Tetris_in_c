#include <stdio.h>
#include <stdlib.h>

int add(int,int);
int mul(int,int);
int sub(int,int);
int divide(int,int);

int main(void) {
    int (*func_ptr_arr[4])(int, int) = {add, mul, sub, divide};

    printf("%d", (*func_ptr_arr[0])(2,3));
    int result = func_ptr_arr[0](2, 3);

    printf("%d",result);

    return 0;
}

int add(int a, int b) {return a+b;}
int mul(int a, int b) {return a*b;}
int sub(int a, int b) {return a-b;}
int divide(int a, int b) {return a/b;}
