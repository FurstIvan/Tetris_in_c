#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_VALUE 1000
#define WORD_BITS 64
#define BITMAP_SIZE ((MAX_VALUE + WORD_BITS) / WORD_BITS)


static uint64_t bitmap[BITMAP_SIZE];

static int bitmap_test(size_t x) {
    size_t word = x / WORD_BITS;
    size_t bit  = x % WORD_BITS;
    return (bitmap[word] & (1ULL << bit)) != 0;
}

static void bitmap_set(size_t x) {
    size_t word = x / WORD_BITS;
    size_t bit  = x % WORD_BITS; 
    bitmap[word] |= (1ULL << bit);
}

int main(void) {
    int arr[] = {5, 12, 7, 5, 99, 12, 1000, 7, 42, 42, 3};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    printf("Duplicates: ");

    for (size_t i = 0; i < n; i++) {
        int value = arr[i];

        if (value < 0 || value > MAX_VALUE) {
            printf("\nError: value %d is out of range 0..%d\n", value, MAX_VALUE);
            return 1;
        }

        if (bitmap_test((size_t)value)) {
            printf("%d ", value);
        } else {
            bitmap_set((size_t)value);
        }
    }

    printf("1    : %zu bits\n", sizeof(1)    * CHAR_BIT);
    printf("1U   : %zu bits\n", sizeof(1U)   * CHAR_BIT);
    printf("1UL  : %zu bits\n", sizeof(1UL)  * CHAR_BIT);
    printf("1ULL : %zu bits\n", sizeof(1ULL) * CHAR_BIT);



    printf("sizeof(unsigned long)      = %zu bytes (%zu bits)\n",
           sizeof(unsigned long),
           sizeof(unsigned long) * CHAR_BIT);

    printf("sizeof(unsigned long long) = %zu bytes (%zu bits)\n",
           sizeof(unsigned long long),
           sizeof(unsigned long long) * CHAR_BIT);

    printf("\n");


    printf("--------------------------------------------------\n");

    uint16_t value = 0b0110001111000001;

    for (int i = 15; i != 0; i--) {
       printf("%d", (value & (1u << i)) != 0);
    }
    printf("\n");    printf("\n");

    int start = 1;
    int sequence_length = 10;

    printf("start = %d sequence:lentght : %d", start, sequence_length);

        printf("\n");    printf("\n");  

    uint16_t mask = (1U << sequence_length) - 1u;
    uint8_t result = (mask << start) & value;

    for (int i = 15; i >= 0; i--) {
        printf("%d", (result & (1u << i)) != 0);
    }
    printf("\n");

    return 0;
}