#include "elevator_hardware.h"
#include "fsm.h"
//#include "orders.h"
//#include "timer.h"

#include <stdio.h>
#include <stdint.h>

// Initializing variables
static position_t fsm_position = UNKNOWN;
static position_t fsm_previous_position = UNKNOWN;
static elev_motor_direction_t fsm_direction = DIRN_STOP;
static state_t fsm_state = INIT;    

// Declaring static local functions
static void m_register_order_press();
static void m_update_position();
static void m_reset_order_lights();

// Functions
        
void fsm(){

    //Checking if the STOP button has been pressed if not in INIT state.
    if (fsm_state != INIT){
        if (elev_get_stop_signal()){
            fsm_state = EMERGENCY_STOP;
        }
    }

    //polling the order buttons and floor sensor when not or init
    if (fsm_state != INIT) {
        m_register_order_press();
    }

    // Updating floor sensor data
    m_update_position();
    
    switch (fsm_state) {

        case INIT:
            // setting the motor direction upwards and keeping in state until hit floor.
            //queue_reset_queue()
            m_reset_order_lights();
            if (fsm_position != UNKNOWN) {
                fsm_state = IDLE;
                fsm_direction = DIRN_STOP;
                break;
            }
            elev_set_motor_direction(DIRN_UP);
            break;
            
        case IDLE:
            elev_set_motor_direction(DIRN_STOP);
            
            //if (queue_is_queue_empty()) {
            //    break;
            //}

            //elev_motor_direction_t order_direction = queue_find_next_direction();
            //if (order_direction == DIRN_STOP) {
            //    fsm_state = OPEN_DOOR;
            //    break;
            //}
            //elev_set_motor_direction(order_direction);
            fsm_state = MOVING;
            fsm_previous_position = fsm_position;
            //fsm_direction = order_direction;
            break;

        case MOVING:
            if (fsm_previous_position == fsm_position) {
                break;
            }
            //queue_should_stop();
            fsm_previous_position = fsm_position;
            break;
        
        case OPEN_DOOR:
            elev_set_motor_direction(DIRN_STOP);
            fsm_direction = DIRN_STOP;
            elev_set_door_open_lamp(1);
            //timer_start_timer();
            //if (timer_is_timer_expired()) {
            //    fsm_state = IDLE;
            //    elev_set_door_open_lamp(0);
            //}
            break;
        
        case EMERGENCY_STOP:
            //queue_reset_queue();
            m_reset_order_lights();
            elev_set_motor_direction(DIRN_STOP);
            if (elev_get_floor_sensor_signal() != -1) {
                elev_set_door_open_lamp(1); 
            }

            //busy wait until button is released
            while (elev_get_stop_signal()){}
            
            if (elev_get_floor_sensor_signal() == -1) {
                fsm_position = IDLE;
            } else {
                fsm_position = OPEN_DOOR;
            }
            
            elev_set_stop_lamp(0);
            break;
    
        default:
            break;
    }
}


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
    int8_t position_incrementer = 0;

    ///////////////////////// TO BE DELETED
    position_t fsm_previous_position = fsm_position;
    ///////////////////////// TO BE DELETED

    //Sets the floor position directly if on floor
    if (floor_sensor != -1) {
        fsm_position = floor_sensor*2; //multiplied with 2 as enum has between positions
    } else {
        if (fsm_direction == DIRN_UP){
            position_incrementer = 1;
        }else {
            position_incrementer = -1;
        }
        switch (fsm_position) {

            case FLOOR_0:
            case FLOOR_1:
            case FLOOR_2:
            case FLOOR_3:
                fsm_position += position_incrementer;
                break;
            
            //Does nothing if in these states
            case UNKNOWN:
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

static void m_reset_order_lights(){
    for (int8_t button = 0; button < N_BUTTONS; button++) {
        for (int8_t floor = 0; floor < N_FLOORS; floor++) {
            elev_set_button_lamp(button, floor, 0);
            }
        }
    }