#include "elevator_hardware.h"
#include "fsm.h"
//#include "orders.h"
//#include "timer.h"

#include <stdio.h>
#include <stdint.h>

// Initializing variables
static position_t fsm_position = UNKNOWN;
static elev_motor_direction_t fsm_direction = DIRN_STOP;
static state_t fsm_state = INIT;    

// Functions

static void m_register_order_press(){
    for (int8_t button = 0; button < N_BUTTONS; button++) {
        for (int8_t floor = 0; floor < N_FLOORS; floor++) {
            int8_t button_signal = elev_get_button_signal(button, floor);
            if (button_signal == 1) {
                printf("add_order called\n");
                //queue_set_order(button, floor);
                elev_set_button_lamp(button, floor, 1);
            }
        }
    }
}

static void m_update_position() {
    int8_t floor_sensor = elev_get_floor_sensor_signal();    

    ///////////////////////// TO BE DELETED
    position_t fsm_previous_position = fsm_position;
    ///////////////////////// TO BE DELETED

    //Sets the floor position directly if on floor
    if (floor_sensor != -1) {
        fsm_position = floor_sensor;
    } else {
        switch (fsm_position) {

            case UNKNOWN:
                //Does nothing
                break;

            case FLOOR_0:
                fsm_position = BETWEEN_0_AND_1;
                break;

            case FLOOR_1:
                if (fsm_direction == DIRN_UP) {
                    fsm_position = BETWEEN_1_AND_2;
                } else{
                    fsm_position = BETWEEN_0_AND_1;
                }

            case FLOOR_2:
                if (fsm_direction == DIRN_UP) {
                    fsm_position = BETWEEN_2_AND_3;
                } else{
                    fsm_position = BETWEEN_1_AND_2;
                }

            case FLOOR_3:
                fsm_position = BETWEEN_2_AND_3;
                break;
            
            //Does nothing if in these states
            case BETWEEN_0_AND_1:
            case BETWEEN_1_AND_2:
            case BETWEEN_2_AND_3:
                break;
    
            default:
                break;
        }
    } 
    ///////////////////TO BE DELETED
    if (fsm_previous_position != fsm_position) {
        printf("state change from %d", fsm_previous_position);
        printf("  to state %d", fsm_position);
        printf("\n");
    }
    ///////////////////TO BE DELETED
}
        
void fsm(){
    //polling the order buttons
    m_register_order_press();
    m_update_position();

    switch (fsm_state) {

        case INIT:
            // setting the motor direction upwards and keeping in state until hit floor.
            if (fsm_position != UNKNOWN) {
                fsm_state = IDLE;
                break;
            }
            elev_set_motor_direction(DIRN_UP);
            break;
            
        case IDLE:
            elev_set_motor_direction(DIRN_STOP);
            break;
    
        default:
            break;
    }
}