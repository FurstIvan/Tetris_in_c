#include <stdio.h>

#define MAKE_VARIABLE_NAME(name, number) name ## number
#define MAKE_FUNC_NAME(name,number) name ## number
#define MAKE_FUNC(name, number, type_1, type_2, type_) int MAKE_FUNC_NAME(name, number) (type_1 a, type_2 b) { \
                                                    return a type_ b;                                          \ 
                                                    }

#define USE_FUNC(name, number, var_1, var_2) MAKE_FUNC_NAME(name,number)(var_1, var_2) 

int MAKE_VARIABLE_NAME(score, 1);

MAKE_FUNC(func, 1, int, int, +)


// ----------------------

#define LIST_OF_VARIABLES \
    X(value1) \
    X(value2) \
    X(value3)

#define X(name) int name; // defines this creation creates --> int value1 
LIST_OF_VARIABLES         // when we call this it generates: int value1 int value2 int value3
#undef X                  // undefine it

void print_variables(void)
{
#define X(name) printf("%s = %d\n", #name, name); // macro stringification
    LIST_OF_VARIABLES
#undef X
}

int main(void) {

    int result = USE_FUNC(func, 1, 1, 2);
    printf("%d\n",result);

    print_variables();

    return 0;
}