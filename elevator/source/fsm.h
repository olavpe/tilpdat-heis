#pragma once

typedef enum position_t {
    FLOOR_0,
    FLOOR_1,
    FLOOR_2,
    FLOOR_3,
    BETWEEN_0_AND_1,    
    BETWEEN_1_AND_2,
    BETWEEN_2_AND_3,
    UNKNOWN
} position_t;

typedef enum state_t {
    INIT,
    IDLE,
    MOVING, 
    OPEN_DOOR,
    EMERGENGY_STOP 
} state_t; 

// functions
void fsm();
//static void m_register_order_press();
//static void m_update_position();
