#pragma once

#define N_POSITIONS 7

typedef enum {
    FLOOR_0,
    BETWEEN_0_AND_1,    
    FLOOR_1,
    BETWEEN_1_AND_2,
    FLOOR_2,
    BETWEEN_2_AND_3,
    FLOOR_3,
    UNKNOWN
} position_t;

typedef enum {
    INIT,
    IDLE,
    MOVING, 
    OPEN_DOOR,
    EMERGENCY_STOP 
} state_t; 


// functions
void fsm();
//static void m_register_order_press();
//static void m_update_position();
