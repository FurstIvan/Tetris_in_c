#include <stdio.h>
#include <stdlib.h>

int add(int, int); 
int sub(int, int);
int mul(int, int);

int main(void) {
    int result;

    int (*func_ptr)(int, int);
    func_ptr = add;
    result = (*func_ptr)(1,2);
    result = func_ptr(1,2);

    int (*func_arr[3])(int, int) = {add, sub, mul};   // func_ptr array 
    result = (func_arr[0])(2,1);
    result = func_arr[0](10, 5);

    func_ptr_arr          // array
    func_ptr_arr[3]       // array of 3
    *func_ptr_arr[3]      // array of 3 pointers
    (*func_ptr_arr[3])()  // array of 3 pointers to functions
    int (*func_ptr_arr[3])(int, int)

    return 0;
}

int add(int a, int b) {return a + b;}
int sub(int a, int b) {return a - b;}
int mul(int a, int b) {return a * b;}