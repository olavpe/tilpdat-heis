#include <stdio.h>
#include <stdint.h>

#include "queue.h"
#include "elev.h"


//making a 2D array of orders with dimension 3(row)x4(col) 
static int queue_array[N_BUTTONS][N_FLOORS]; //static? bør eg sette alle verdiar til 0?

//deletes all orders in queue_array, and setting all orders to the initial value 0
void queue_reset_queue(){
    //setting default values in 2D array
    int8_t button, floor;
    for(button = 0; button < N_BUTTONS ; button++){
        for(floor = 0; floor < N_FLOORS; floor++){
            queue_array[button][floor] == 0;
        }
    }
}

//deletes an order - used when an order is fullfilled
void queue_delete_order(position_t current_position){ 
    int8_t button;
    for(button = 0; button < N_BUTTONS; button++){
        queue_array[button][current_position] == 0;
    }
}

int queue_get_order(elev_button_type_t button, position_t floor){//finst det ein enum med knappar?
    return queue_array[button][floor];
    //er det problemastisk at enum position_t også inneheld mellometasjane?
}

void queue_set_order(elev_button_type_t button, position_t floor){
    assert_buttons();
    queue_array[button][floor] = 1;
}

//if the expressions evaluates to false the assert() will display an error message
//brukast også i add_order...
//void assert_buttons();
//hjelpefunksjon til queue_get_next_direction
//elev_motor_direction_t choose_direction_based_on_priority(elev_motor_direction_t last_direction, int8_t orders_above, int8_t orders_below);

elev_motor_direction_t queue_get_next_direction(position_t current_position, elev_motor_direction_t last_direction){
    queue_assert();

    int8_t floor;
    int8_t button;
    //lagar ny array som skal innhalde summen av kolonnene i queue_array - altså om det er bestillingar i ein etasje.
    static int num_orders_array[N_FLOORS] = {0, 0, 0, 0};
    for (floor = 0; floor < N_FLOORS; floor++){
        for (button = 0; button < N_BUTTONS; button++){
            num_orders_array[floor] += queue_array[button][floor];
        }
    }
    //berre interessert i om order_array inneheld 0 eller < 0.
    //
    int8_t orders_above = 0;
    int8_t orders_below = 0;

    switch(current_position){
        case FLOOR_0:
            orders_above = num_orders_array[FLOOR_1] + num_orders_array[FLOOR_2] + num_orders_array[FLOOR_3];
            choose_direction_based_on_priority(last_direction, orders_above, orders_below);
            break;

        case BETWEEN_FLOOR_0_AND_1:
            if (last_direction == DIRN_UP){
                return DIRN_UP;
            }
            else if (last_direction == DIRN_DOWN){
                return DIRN_DOWN;
            }
            else if (last_direction == DIRN_STOP){
                orders_above = num_orders_array[FLOOR_1] + num_orders_array[FLOOR_2] + num_orders_array[FLOOR_3];
                orders_below = num_orders_array[FLOOR_0];
                choose_direction_based_on_priority(last_direction, orders_above, orders_below);
            }
            break;

        case FLOOR_1:
            orders_above = num_orders_array[FLOOR_2] + num_orders_array[FLOOR_3];
            orders_below = num_orders_array[FLOOR_0];
            choose_direction_based_on_priority(last_direction, orders_above, orders_below);
            break;

        case BETWEEN_FLOOR_1_AND_2:
            if (last_direction == DIRN_UP){
                return DIRN_UP;
            }
            else if (last_direction == DIRN_DOWN){
                return DIRN_DOWN;
            }
            else if (last_direction == DIRN_STOP){
                orders_above = num_orders_array[FLOOR_2] + num_orders_array[FLOOR_3];
                orders_below = num_orders_array[FLOOR_0] + num_orders_array[FLOOR_1];
                choose_direction_based_on_priority(last_direction, orders_above, orders_below);
            }
            break;

        case FLOOR_2:
            orders_above = num_orders_array[FLOOR_3];
            orders_below = num_orders_array[FLOOR_0] + num_orders_array[FLOOR_1];
            choose_direction_based_on_priority(last_direction, orders_above, orders_below);
            break;

        case BETWEEN_FLOOR_2_AND_3:
            if (last_direction == DIRN_UP){
                return DIRN_UP;
            }
            else if (last_direction == DIRN_DOWN){
                return DIRN_DOWN;
            }
            else if (last_direction == DIRN_STOP){
                orders_above = num_orders_array[FLOOR_3];
                orders_below = num_orders_array[FLOOR_0] + num_orders_array[FLOOR_1] + num_orders_array[FLOOR_2];
                choose_direction_based_on_priority(last_direction, orders_above, orders_below);
            }
            break;

        case FLOOR_3:
            orders_above = num_orders_array[FLOOR_0] + num_orders_array[FLOOR_1] + num_orders_array[FLOOR_2];
            choose_direction_based_on_priority(last_direction, orders_above, orders_below);
            break;
    }
}

void assert_buttons(){
    int8_t no_button_up = 0, no_button_down = 1;
    assert(queue_array[no_button_up][FLOOR_3] == 0);
    assert(queue_array[no_button_down][FLOOR_0] == 0);
}

elev_motor_direction_t choose_direction_based_on_priority(elev_motor_direction_t last_direction, int8_t orders_above, int8_t orders_below){
    switch(last_direction){
        case DIRN_UP:
            if (orders_above > 0){
                return DIRN_UP;
            } else if (orders_below > 0){
                return DIRN_DOWN;
            } else {
                return DIRN_STOP;
            }
            break;
        case DIRN_DOWN:
            if (orders_below > 0){
                return DIRN_DOWN;
            } else if (orders_above > 0){
                return DIRN_UP;
            } else {
                return DIRN_STOP;
            }
            break;
        case DIRN_STOP:  //er vi einige om at default retning er opp eller ned i forhold til dirn_stop??
        //eller er det slik at vi treng å lagre last_last_direction også for å finne kva retning som skal brukast etter dirn_stop
        //om dette er den retninga den har i idle når heisen stopppar og tek på folk?? i det tilfellet vert dette meir komplisert
            if (orders_above > 0){
                return DIRN_UP;
            } else if (orders_below > 0){
                return DIRN_DOWN;
            } else {
                return DIRN_STOP;
            }
            break;
    }
}
