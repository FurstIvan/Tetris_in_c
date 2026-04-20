#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main () {
    int* arr = malloc(sizeof(int) * 4);
    int* temp = arr;

    for (int i = 0; i < 4; i++) {
        int arr_val = *(temp + i);
    }

    uint16_t square[4] = {0,0,0,0};

    for (int i = 0; i < 4; i++) {
        if (i == 0){
            square[i] |= (3 << 1);
            square[i] |= (3 << 5);
            square[i] |= (3 << 9);
            square[i] |= (3 << 13);
        }
        if (i == 1) {
            square[i] |= (3 << 1);
            square[i] |= (3 << 5);
            square[i] |= (3 << 9);
            square[i] |= (3 << 13);          
        }
        for (int j = 0; j < 16; j++ ) {
            if (j % 4 == 0) {
                printf("|");
            }
            if ( (square[i] & (1 << j) ) != 0) {
                printf("1");
            }
            else {
                printf("0");
            }
        }
        printf("\t\t%d", square[i]);
        printf("\n");

    }

    uint16_t l_shape_left[4] = {0,0,0,0};

    printf("\n \n \n\n L_SHAPE-LEFT\n\n");

    for (int i = 0; i < 4; i++) {
        if (i == 0){
            l_shape_left[i] |= (0 << 12);
            l_shape_left[i] |= (0 << 8);
            l_shape_left[i] |= (0 << 4);
            l_shape_left[i] |= (4 << 0);
        }
        if (i == 1) {
            l_shape_left[i] |= (7 << 12);
            l_shape_left[i] |= (6 << 8);
            l_shape_left[i] |= (2 << 4);
            l_shape_left[i] |= (4 << 0);            
        }
        if (i == 2) {
            l_shape_left[i] |= (4 << 12);
            l_shape_left[i] |= (2 << 8);
            l_shape_left[i] |= (14 << 4);
            l_shape_left[i] |= (6 << 0);            
        }
        if (i == 3) {
            l_shape_left[i] |= (0 << 12);
            l_shape_left[i] |= (2 << 8);
            l_shape_left[i] |= (0 << 4);
            l_shape_left[i] |= (0 << 0);            
        }
        for (int j = 0; j < 16; j++ ) {
            if (j % 4 == 0) {
                printf("|");
            }
            if ( (l_shape_left[i] & (1 << j) ) != 0) {
                printf("1");
            }
            else {
                printf("0");
            }
        }
        printf("\t\t%d", l_shape_left[i]);
        printf("\n");

    }

    uint16_t l_shape_right[4] = {0,0,0,0};

    printf("\n \n \n\n L_SHAPE-RIGHT\n\n");

    for (int i = 0; i < 4; i++) {
        if (i == 0){
            l_shape_right[i] |= (0 << 12);
            l_shape_right[i] |= (0 << 8);
            l_shape_right[i] |= (0 << 4);
            l_shape_right[i] |= (2 << 0);
        }
        if (i == 1) {
            l_shape_right[i] |= (4 << 12);
            l_shape_right[i] |= (6 << 8);
            l_shape_right[i] |= (14 << 4);
            l_shape_right[i] |= (2 << 0);            
        }
        if (i == 2) {
            l_shape_right[i] |= (7 << 12);
            l_shape_right[i] |= (4 << 8);
            l_shape_right[i] |= (2 << 4);
            l_shape_right[i] |= (6 << 0);            
        }
        if (i == 3) {
            l_shape_right[i] |= (0 << 12);
            l_shape_right[i] |= (4 << 8);
            l_shape_right[i] |= (0 << 4);
            l_shape_right[i] |= (0 << 0);            
        }
        for (int j = 0; j < 16; j++ ) {
            if (j % 4 == 0) {
                printf("|");
            }
            if ( (l_shape_right[i] & (1 << j) ) != 0) {
                printf("1");
            }
            else {
                printf("0");
            }
        }
        printf("\t\t%d", l_shape_right[i]);
        printf("\n");

    }


    uint16_t I_shape[4] = {0,0,0,0};

    printf("\n \n \n\n I_shape\n\n");

    for (int i = 0; i < 4; i++) {
        if (i == 0){
            I_shape[i] |= (0 << 12);
            I_shape[i] |= (2 << 8);
            I_shape[i] |= (0 << 4);
            I_shape[i] |= (2 << 0);
        }
        if (i == 1) {
            I_shape[i] |= (15 << 12);
            I_shape[i] |= (2 << 8);
            I_shape[i] |= (15 << 4);
            I_shape[i] |= (2 << 0);            
        }
        if (i == 2) {
            I_shape[i] |= (0 << 12);
            I_shape[i] |= (2 << 8);
            I_shape[i] |= (0 << 4);
            I_shape[i] |= (2 << 0);            
        }
        if (i == 3) {
            I_shape[i] |= (0 << 12);
            I_shape[i] |= (2 << 8);
            I_shape[i] |= (0 << 4);
            I_shape[i] |= (2 << 0);            
        }
        for (int j = 0; j < 16; j++ ) {
            if (j % 4 == 0) {
                printf("|");
            }
            if ( (I_shape[i] & (1 << j) ) != 0) {
                printf("1");
            }
            else {
                printf("0");
            }
        }
        printf("\t\t%d", I_shape[i]);
        printf("\n");

    }

    uint16_t t_shape[4] = {0,0,0,0};

    printf("\n \n \n\n T_SHAPE\n\n");

    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            t_shape[i] |= (0 << 12);
            t_shape[i] |= (0 << 8);
            t_shape[i] |= (0 << 4);
            t_shape[i] |= (0 << 0);
        }
        if (i == 1) {
            t_shape[i] |= (4 << 12);
            t_shape[i] |= (7 << 8);
            t_shape[i] |= (2 << 4);
            t_shape[i] |= (2 << 0);            
        }
        if (i == 2) {
            t_shape[i] |= (6 << 12);
            t_shape[i] |= (2 << 8);
            t_shape[i] |= (6 << 4);
            t_shape[i] |= (7 << 0);            
        }
        if (i == 3) {
            t_shape[i] |= (4 << 12);
            t_shape[i] |= (0 << 8);
            t_shape[i] |= (2 << 4);
            t_shape[i] |= (0 << 0);            
        }
        for (int j = 0; j < 16; j++ ) {
            if (j % 4 == 0) {
                printf("|");
            }
            if ( (t_shape[i] & (1 << j) ) != 0) {
                printf("1");
            }
            else {
                printf("0");
            }
        }
        printf("\t\t%d", t_shape[i]);
        printf("\n");
    }

    uint16_t left_z[4] = {0,0,0,0};

    printf("\n \n \n\n LEFT_Z_SHAPE\n\n");

    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            left_z[i] |= (4 << 12);
            left_z[i] |= (0 << 8);
            left_z[i] |= (4 << 4);
            left_z[i] |= (0 << 0);
        }
        if (i == 1) {
            left_z[i] |= (6 << 12);
            left_z[i] |= (3 << 8);
            left_z[i] |= (6 << 4);
            left_z[i] |= (3 << 0);            
        }
        if (i == 2) {
            left_z[i] |= (2 << 12);
            left_z[i] |= (6 << 8);
            left_z[i] |= (2 << 4);
            left_z[i] |= (6 << 0);            
        }
        if (i == 3) {
            left_z[i] |= (0 << 12);
            left_z[i] |= (0 << 8);
            left_z[i] |= (0 << 4);
            left_z[i] |= (0 << 0);            
        }
        for (int j = 0; j < 16; j++ ) {
            if (j % 4 == 0) {
                printf("|");
            }
            if ( (left_z[i] & (1 << j) ) != 0) {
                printf("1");
            }
            else {
                printf("0");
            }
        }
        printf("\t\t%d", left_z[i]);
        printf("\n");
    }

    uint16_t right_z[4] = {0,0,0,0};

    printf("\n \n \n\n RIGHT_Z\n\n");

    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            right_z[i] |= (2 << 12);
            right_z[i] |= (0 << 8);
            right_z[i] |= (2 << 4);
            right_z[i] |= (0 << 0);
        }
        if (i == 1) {
            right_z[i] |= (6 << 12);
            right_z[i] |= (12 << 8);
            right_z[i] |= (6 << 4);
            right_z[i] |= (12 << 0);            
        }
        if (i == 2) {
            right_z[i] |= (4 << 12);
            right_z[i] |= (6 << 8);
            right_z[i] |= (4 << 4);
            right_z[i] |= (6 << 0);            
        }
        if (i == 3) {
            right_z[i] |= (0 << 12);
            right_z[i] |= (0 << 8);
            right_z[i] |= (0 << 4);
            right_z[i] |= (0 << 0);            
        }
        for (int j = 0; j < 16; j++ ) {
            if (j % 4 == 0) {
                printf("|");
            }
            if ( (right_z[i] & (1 << j) ) != 0) {
                printf("1");
            }
            else {
                printf("0");
            }
        }
        printf("\t\t%d", right_z[i]);
        printf("\n");
    }


    return 0;
}