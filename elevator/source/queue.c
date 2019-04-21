#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "queue.h"
#include "elevator_hardware.h"
#include "fsm.h"

// Initializing variables
static int queue_array[N_BUTTONS][N_FLOORS];

//Declaring static local functions
static void m_assert_buttons();
static elev_motor_direction_t m_choose_direction_based_on_priority(elev_motor_direction_t last_direction, int orders_above, int orders_below, int order_same_floor);
static elev_button_type_t m_get_button_matching_direction(elev_motor_direction_t fsm_direction);

// Functions
void queue_reset_queue(){
    floor_t floor;
    elev_button_type_t button;
    for (floor = 0; floor < N_FLOORS ; floor++){
        for (button = 0; button < N_BUTTONS; button++){
            queue_array[button][floor] = 0;
            elev_set_button_lamp(button, floor, 0);
        }
    }
}

void queue_delete_order(floor_t floor){ 
    elev_button_type_t button;
    for (button = 0; button < N_BUTTONS; button++){
        queue_array[button][floor] = 0;
        elev_set_button_lamp(button,floor, 0);
    }
}

void queue_set_order(elev_button_type_t button, floor_t floor){
    queue_array[button][floor] = 1;
    m_assert_buttons();
}

bool queue_is_queue_empty(){
    int orders = 0;
    floor_t floor;
    elev_button_type_t button;
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
    int orders_above = 0, orders_below = 0, order_same_floor = 0;
    floor_t floor;
    elev_button_type_t button;

    m_assert_buttons();
    // Loops through the queue array and counts order relative to the position of the elevator.
    for (floor = 0; floor < N_FLOORS; floor++){
        for (button = 0; button < N_BUTTONS; button++){
            if (2*floor < current_position){ // floor is mulitplied by 2 due to relationship between position_t and floor_t
                orders_below += queue_array[button][floor];
            } else if (2*floor > current_position){
                orders_above += queue_array[button][floor];
            } else if (2*floor == current_position){
                order_same_floor += queue_array[button][floor];
            }
        }
    }

    // Position found based on the different direction 
    elev_motor_direction_t next_direction = m_choose_direction_based_on_priority(last_direction, orders_above, orders_below, order_same_floor);
    return next_direction;
}

bool queue_should_stop(position_t fsm_position, floor_t fsm_floor, elev_motor_direction_t fsm_direction){
    if (queue_get_next_direction(fsm_position, fsm_direction) != fsm_direction) { // If there are no orders in the curret direction of travel it will stop
        return true;
    }
    if (queue_array[BUTTON_COMMAND][fsm_floor] == 1) {
        return true;
    }
    elev_button_type_t button_in_direction = m_get_button_matching_direction(fsm_direction);
    if (queue_array[button_in_direction][fsm_floor] == 1) {
        return true;
    }
    return false;
}


//if the expressions evaluates to false the assert() will display an error message
static void m_assert_buttons(){
    int floor_0 = 0, floor_3 = 3;
    assert(queue_array[BUTTON_CALL_UP][floor_3] == 0);
    assert(queue_array[BUTTON_CALL_DOWN][floor_0] == 0);
}

// Function that determines the next direction based on the travel priority and the orders relative to the elevator.
static elev_motor_direction_t m_choose_direction_based_on_priority(elev_motor_direction_t last_direction, int orders_above, int orders_below, int order_same_floor){
    elev_motor_direction_t next_direction;
    switch(last_direction){

        case DIRN_UP:
            if (orders_above > 0){
                next_direction = DIRN_UP;
            } else if (orders_below > 0){
                next_direction = DIRN_DOWN;
            } else if (order_same_floor > 0){
                next_direction = DIRN_STOP;
            }
            break;

        case DIRN_DOWN:
            if (orders_below > 0){
                next_direction = DIRN_DOWN;
            } else if (orders_above > 0){
                next_direction = DIRN_UP;
            } else if (order_same_floor > 0) {
                next_direction = DIRN_STOP;
            } 
            break;

        case DIRN_STOP:
            break;
    }
    return next_direction;
}

// Function that ties the direction of travel to the button directions of the elevator.
static elev_button_type_t m_get_button_matching_direction(elev_motor_direction_t fsm_direction){
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