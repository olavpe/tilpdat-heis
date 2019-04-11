#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "queue.h"
#include "fsm.h"


//hjelpefunksjonar
void m_assert_buttons();
int * m_calculate_orders_per_floor();
elev_motor_direction_t m_choose_direction_based_on_priority(elev_motor_direction_t last_direction, int8_t orders_above, int8_t orders_below, int8_t order_same_floor);
elev_button_type_t m_get_button_matching_direction(elev_motor_direction_t fsm_direction);


static int queue_array[N_BUTTONS][N_FLOORS];


void queue_reset_queue(){
    int8_t button, floor;
    for(button = 0; button < N_BUTTONS ; button++){
        for(floor = 0; floor < N_FLOORS; floor++){
            queue_array[button][floor] = 0;
        }
    }
}

void queue_delete_order(position_t current_position){ 
    int8_t button;
    for(button = 0; button < N_BUTTONS; button++){
        queue_array[button][current_position] = 0;
    }
}

int queue_get_order(elev_button_type_t button, position_t floor){
    return queue_array[button][floor];
    //er det problemastisk at enum position_t også inneheld mellometasjane?
    //fungerer så lenge denne berre vert brukt i heiletasjane!
}

void queue_set_order(elev_button_type_t button, position_t floor){
    m_assert_buttons();
    queue_array[button][floor] = 1;
}

bool queue_is_queue_empty(){
    int8_t orders = 0;
    int8_t floor, button;
    for (floor = 0; floor < N_FLOORS; floor++){
        for (button = 0; button < N_BUTTONS; button++){
            orders += queue_array[button][floor];
        }
    }
    if (orders == 0){
        return true;
    } else {
        return false;
    } 
}

elev_motor_direction_t queue_get_next_direction(position_t current_position, elev_motor_direction_t last_direction){
    int * p_num_orders_array;
    p_num_orders_array = m_calculate_orders_per_floor();

    int8_t orders_above = 0;
    int8_t orders_below = 0;
    int8_t order_same_floor = 0;
    int8_t position;
    
    // for(position = (current_position + 1); position < FLOOR_3; position++){
    //     orders_above += p_num_orders_array[position];
    //     //orders_above += *(p_num_orders_array + current_position);
    // }
    // for(position = (current_position - 1); position > FLOOR_0; position--){
    //     //orders_below += *(p_num_orders_array + current_position);
    //     orders_below += p_num_orders_array[position];
    // }
    for(position = FLOOR_0; position < FLOOR_3; position++){
        if (position < current_position){
            orders_below += p_num_orders_array[position];
        } else if (position > current_position){
            orders_above += p_num_orders_array[position];
        } else if (position == current_position){
            order_same_floor = p_num_orders_array[position];
        }
    }
    //order_same_floor = *(p_num_orders_array + current_position);
    //order_same_floor = p_num_orders_array[current_position];

    elev_motor_direction_t next_direction = m_choose_direction_based_on_priority(last_direction, orders_above, orders_below, order_same_floor);
    return next_direction;
}

bool queue_should_stop(position_t fsm_position, elev_motor_direction_t fsm_direction){
    if (queue_get_next_direction(fsm_position, fsm_direction) != fsm_direction) {
        return true;
    }
    if (queue_array[BUTTON_COMMAND][fsm_position] == 1) {
        return true;
    }
    elev_button_type_t button_in_direction = m_get_button_matching_direction(fsm_direction);
    if (queue_array[button_in_direction][fsm_position] == 1) {
        return true;
    }
    return false;
}


//if the expressions evaluates to false the assert() will display an error message
void m_assert_buttons(){
    int8_t no_button_up = 0, no_button_down = 1;
    assert(queue_array[no_button_up][FLOOR_3] == 0);
    assert(queue_array[no_button_down][FLOOR_0] == 0);
}

int * m_calculate_orders_per_floor(){
    m_assert_buttons();
    int8_t fsm_position;
    int8_t button;
    //lagar ny array som skal innhalde summen av kolonnene i queue_array - altså om det er bestillingar i ein etasje, samt at between floors alltid er 0
    static int num_orders_array[N_POSITIONS] = {0, 0, 0, 0, 0, 0, 0};
    for (fsm_position = 0; fsm_position < N_POSITIONS; fsm_position = (fsm_position+2)){
        for (button = 0; button < N_BUTTONS; button++){
            num_orders_array[fsm_position] += queue_array[button][fsm_position];
        }
    }

    assert(num_orders_array[BETWEEN_0_AND_1] == 0);
    assert(num_orders_array[BETWEEN_1_AND_2] == 0);
    assert(num_orders_array[BETWEEN_2_AND_3] == 0);

    return num_orders_array;
}

elev_motor_direction_t m_choose_direction_based_on_priority(elev_motor_direction_t last_direction, int8_t orders_above, int8_t orders_below, int8_t order_same_floor){
    elev_motor_direction_t next_direction;
    switch(last_direction){
        case DIRN_UP:
            if (orders_above > 0){
                next_direction = DIRN_UP;
                printf("case D_UP: direction is UP\n");
            } else if (orders_below > 0){
                next_direction = DIRN_DOWN;
                printf("case D_UP: direction is DOWN\n");
            //her vil det vere eit tilfelle når queue er tom at next_dir ikkje har ein gyldig verdi
            //sjekke på dette? men er DIRN_STOP gitt at ein stoppar OG opnar døra, for i dette tilfellet
            //vil vi holde døra igjen.
            } else if (order_same_floor > 0){
                next_direction = DIRN_STOP;
                printf("case D_UP: direction is STOP\n");
            }
            break;
        case DIRN_DOWN:
            if (orders_below > 0){
                next_direction = DIRN_DOWN;
                printf("case D_DOWN: direction is DOWN\n");
            } else if (orders_above > 0){
                next_direction = DIRN_UP;
                printf("case D_DOWN: direction is UP\n");
            } else if (order_same_floor > 0) {
                next_direction = DIRN_STOP;
                printf("case D_DOWN: direction is STOP\n");
            } 
            break;
        case DIRN_STOP:
            printf("should not occur\n");
            break;
    }
    return next_direction;
}

elev_button_type_t m_get_button_matching_direction(elev_motor_direction_t fsm_direction){
   switch (fsm_direction) {
       case DIRN_DOWN:
            return BUTTON_CALL_DOWN;
   
       case DIRN_UP:
            return BUTTON_CALL_UP;

       default:
            return BUTTON_COMMAND;
            break;
   } 
}