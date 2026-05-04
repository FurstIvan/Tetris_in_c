#include "data.h"

                            // we need the 0 because how we implemented grav_ramping later on
Gravity grav_ramp_states[] = {0,RAMP0,RAMP1,RAMP2,RAMP3,RAMP4,RAMP5,RAMP6,RAMP7,RAMP8,RAMP9,END};

                            //single, double, triple, tetris
uint8_t simple_score_board[] = {0,1,3,5,8};

                                                                                                                                                                                                    
Tetrimino_Color color_map[t_shape + 2] = { 
    [left_l_shape] = {253u, 63u, 89u}, 
    [right_l_shape] = {255u, 200u, 46u}, 
    [square] = {254u, 251u, 52u}, 
    [I_block] = {83u, 218u, 63u}, 
    [left_z_block] = {1u, 237u, 250u}, 
    [right_z_block] = {221u, 10u, 178u}, 
    [t_shape] = {234u, 20u, 28u},
    [t_shape + 1] = {225u, 248u, 220u}}; // locked board square color  

const Rotation_mask all_shape_Tetrimino_State_Map[t_shape + 1][Rotated_thrice +1] = { 
    [left_l_shape] = {
        {
            .rows = {
                0b0001,
                0b0001,
                0b0011,
                0b0000
            },
            .width = 2,
            .height = 3
        }, 
        {
            .rows = {
                0b0100,
                0b0111,
                0b0000,
                0b0000
            },
            .width = 3,
            .height = 2
        }, 
        {
            .rows = {
                0b0011,
                0b0010,
                0b0010,
                0b0000
            },
            .width = 2,
            .height = 3
        }, {
            .rows = {
                0b0111,
                0b0001,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        }
    },
    [right_l_shape] = {
        {
            .rows = {
                0b0010,
                0b0010,
                0b0011,
                0b0000
            },
            .width = 2,
            .height = 3
        },
        {
            .rows = {
                0b0111,
                0b0100,
                0b0000,
                0b0000
            },
            .width = 3,
            .height = 2
        }, {
            .rows = {
                0b0011,
                0b0001,
                0b0001,
                0b0000
            },
            .width = 2,
            .height = 3
        }, {
            .rows = {
                0b0001,
                0b0111,
                0b0000,
                0b0000
            },
            .width = 3,
            .height = 2
        }
    },      
    [square] = {  
        {
            .rows = {
                0b0011,
                0b0011,
                0b0000,
            },
            .height = 2,
            .width = 2
        },
        {
            .rows = {
                0b0011,
                0b0011,
                0b0000,
            },
            .height = 2,
            .width = 2
        },
        {
            .rows = {
                0b0011,
                0b0011,
                0b0000,
            },
            .height = 2,
            .width = 2
        },
        {
            .rows = {
                0b0011,
                0b0011,
                0b0000,
            },
            .height = 2,
            .width = 2
        }        
    },      
    [I_block] = {       
        {
            .rows = {
                0b0001,
                0b0001,
                0b0001,
                0b0001
            },
            .height = 4,
            .width = 1
        },
        {
            .rows = {
                0b1111,
                0b0000,
                0b0000,
                0b0000
            },
            .height = 1,
            .width = 4
        },
        {
            .rows = {
                0b0001,
                0b0001,
                0b0001,
                0b0001
            },
            .height = 4,
            .width = 1
        },
        {
            .rows = {
                0b1111,
                0b0000,
                0b0000,
                0b0000
            },
            .height = 1,
            .width = 4
        }
    },
    [left_z_block] = {       
        {
            .rows = {
                0b0110,
                0b0011,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0001,
                0b0011,
                0b0010,
                0b0000
            },
            .height = 3,
            .width = 2
        },
        {
            .rows = {
                0b0110,
                0b0011,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0001,
                0b0011,
                0b0010,
                0b0000
            },
            .height = 3,
            .width = 2
        }
    },
    [right_z_block] = {       
        {
            .rows = {
                0b0011,
                0b0110,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0010,
                0b0011,
                0b0001,
                0b0000
            },
            .height = 3,
            .width = 2
        },
        {
            .rows = {
                0b0011,
                0b0110,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0010,
                0b0011,
                0b0001,
                0b0000
            },
            .height = 3,
            .width = 2
        }
    },
    [t_shape] = {       
        {
            .rows = {
                0b0010,
                0b0111,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0010,
                0b0011,
                0b0010,
                0b0000
            },
            .height = 3,
            .width = 2
        },
        {
            .rows = {
                0b0111,
                0b0010,
                0b0000,
                0b0000
            },
            .height = 2,
            .width = 3
        },
        {
            .rows = {
                0b0001,
                0b0011,
                0b0001,
                0b0000
            },
            .height = 3,
            .width = 2
        }
    }
};