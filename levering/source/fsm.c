/**
 * @file
 * @brief Implementation of the functions in fsm.h
 */ 

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include "elevator_hardware.h"
#include "fsm.h"
#include "queue.h"
#include "timer.h"


// Initializing variables
/** @brief Local fsm variable used to keep track of the elevators position. */
static position_t fsm_position = UNKNOWN;
/** @brief Local fsm variable used to keep track of the elevators last or current floor. */
static floor_t fsm_floor = ORDER_FLOOR_UNKNOWN;
/** @brief Local fsm variable to keep track of timer responisble for opening the door. */
static time_t fsm_timestamp = 0;
/** @brief Local fsm variable used to keep track of the elevators direction of travel. This can never be DIRN_STOP */
static elev_motor_direction_t fsm_direction = DIRN_UP; 
/** @brief Local fsm variable used to keep track of the elevators state. */
static state_t fsm_state = INIT;

// Declaring static local functions
static void m_register_order_press();
static void m_update_position_and_floor();
static void m_reset_order_lights();

// Functions
        
void fsm(){

    //polling the order buttons and floor sensor when not in INIT
    if (fsm_state != INIT) {
        m_register_order_press();
    }

    // Updating floor and position from sensor data
    m_update_position_and_floor();

    //Checking if the STOP button has been pressed if not in INIT state.
    if (fsm_state != INIT){
        if (elev_get_stop_signal()){
            fsm_state = EMERGENCY_STOP;
        }
    }
    
    switch (fsm_state) {

        case INIT:
            // setting the motor direction upwards and keeping in state until hit floor.
            queue_reset_queue();
            m_reset_order_lights();
            if (fsm_position != UNKNOWN) {
                fsm_state = IDLE;
                break;
            }
            elev_set_motor_direction(DIRN_UP); //default direction when initilizting
            break;
            
        case IDLE:
            elev_set_motor_direction(DIRN_STOP);
            
            if (queue_is_queue_empty()) {
                break; //does nothing and exits case if the queue is empty
            } 

            elev_motor_direction_t order_direction = queue_get_next_direction(fsm_position, fsm_direction);
            if (order_direction == DIRN_STOP) {
                fsm_timestamp = timer_start_timer();
                fsm_state = OPEN_DOOR;
                break;
            }
            elev_set_motor_direction(order_direction);
            fsm_direction = order_direction;
            fsm_state = MOVING;
            break;

        case MOVING:
            switch (fsm_position){
                case FLOOR_0:
                case FLOOR_1:
                case FLOOR_2:
                case FLOOR_3:
                    if (queue_should_stop(fsm_position, fsm_floor, fsm_direction)) {
                        elev_set_motor_direction(DIRN_STOP);
                        fsm_timestamp = timer_start_timer();
                        fsm_state = OPEN_DOOR;
                    }
                    break;

                default:
                    break;
            }
            break;
        
        case OPEN_DOOR:
            elev_set_motor_direction(DIRN_STOP);
            elev_set_door_open_lamp(1);
            if (timer_is_timer_expired(fsm_timestamp)){
                elev_set_door_open_lamp(0);
                queue_delete_order(fsm_floor);
                fsm_state = IDLE;
            }
            break;
        
        case EMERGENCY_STOP:
            queue_reset_queue();
            elev_set_motor_direction(DIRN_STOP);
            elev_set_stop_lamp(1);
            if (elev_get_floor_sensor_signal() != -1) {
                elev_set_door_open_lamp(1); //opens door if elevator is at a floor
            }

            //busy wait until button is released
            while (elev_get_stop_signal()){}
            
            if (elev_get_floor_sensor_signal() == -1) {
                fsm_state = IDLE;
            } else {
                fsm_timestamp = timer_start_timer();
                fsm_state = OPEN_DOOR;
            }
            elev_set_stop_lamp(0);
            break;
    
        default:
            break;
    }
}


static void m_register_order_press(){
    for (elev_button_type_t button = 0; button < N_BUTTONS; button++) {
        for (floor_t floor = 0; floor < N_FLOORS; floor++) {
            elev_button_type_t button_signal = elev_get_button_signal(button, floor);
            if (button_signal == 1) {
                queue_set_order(button, floor);
                elev_set_button_lamp(button, floor, 1);
            }
        }
    }
}

static void m_update_position_and_floor() {
    floor_t floor_sensor = elev_get_floor_sensor_signal();    
    int position_incrementer = 0; //used to determine if elevator is going up or down
    //Sets the floor position directly if on floor
    if (floor_sensor != -1) {
        fsm_floor = floor_sensor;
        fsm_position = fsm_floor*2; //multiplied with 2 as enum has between positions
        elev_set_floor_indicator(fsm_floor);
    } else {
        if (fsm_direction == DIRN_UP){
            position_incrementer = 1;
        }else if (fsm_direction == DIRN_DOWN) {
            position_incrementer = -1;
        } else {
            position_incrementer = 0;
        }

        switch (fsm_position) {

            case FLOOR_0:
            case FLOOR_1:
            case FLOOR_2:
            case FLOOR_3:
                fsm_position += position_incrementer;
                break;

            //No changes for any BETWEEN_FLOORS position og UNKNOWN    
            default:
                break;
        }
    } 
}

static void m_reset_order_lights(){
    for (elev_button_type_t button = 0; button < N_BUTTONS; button++) {
        for (floor_t floor = 0; floor < N_FLOORS; floor++) {
            }
        }
    }
