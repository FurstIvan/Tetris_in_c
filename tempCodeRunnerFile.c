#include <math.h>

int main () {
    int* arr = malloc(sizeof(int) * 4);
    int* temp = arr;

    for (int i = 0; i < 4; i++) {
        int arr_val = *(temp + i);
    }

    pow(1.0, 2.0);

    return 0;
}