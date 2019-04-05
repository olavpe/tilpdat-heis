#include <stdio.h>
#include <stdint.h>

#include "queue.h"
#include "elev.h"


//making a 2D array of orders with dimension 3(row)x4(col)
int queue_array[N_BUTTONS][N_FLOORS];

//deletes all orders in queue_array, and setting all orders to the initial value 0
void queue_delete_queue(){
    //setting default values in 2D array
    int8_t button, floor;
    for(button = 0; button < N_BUTTONS ; button++){
        for(floor = 0; floor < N_FLOORS; floor++){
            queue_array[button][floor] == 0;
        }
    }
}

//deletes an order - used when an order is fullfilled
void queue_delete_order(int current_position){ 
    int8_t button;
    for(button = 0; button < N_BUTTONS; button++){
    queue_array[button][current_position] == 0;
    }
}

//if the expressions evaluates to false the assert() will display an error message
//det er ikkje mogeleg å køyre nedover frå første etg, samt oppover frå fjerde etg
void queue_assert(){
    int8_t button_up = 0, button_down = 1;
    //floor_3 and floor_0 from enum at fsm
    assert(queue_array[button_up][FLOOR_3] == 0);
    assert(queue_array[button_down][FLOOR_0] == 0);
}

int queue_find_closest_order(int current_position, int last_direction){
    queue_assert();
//først finn bestilling i samme etasje, så etasjane i same retning¨
//så finn bestillingar i motsett retning


    
    return ...;//etasje
}

int queue_get_order(){}