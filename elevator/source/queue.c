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