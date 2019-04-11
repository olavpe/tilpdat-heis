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
static position_t fsm_position = UNKNOWN;
static position_t fsm_previous_position = UNKNOWN;
static time_t fsm_timestamp = 0;
static elev_motor_direction_t fsm_direction = DIRN_UP;
//////////////// TO BE DELETED
static elev_motor_direction_t fsm_last_direction = DIRN_UP;
//////////////// TO BE DELETED
static state_t fsm_state = INIT;    

// Declaring static local functions
static void m_register_order_press();
static void m_update_position();
static void m_reset_order_lights();

// Functions
        
void fsm(){

    //polling the order buttons and floor sensor when not or init
    if (fsm_state != INIT) {
        m_register_order_press();
    }

    // Updating floor sensor data
    m_update_position();

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
                //fsm_direction = DIRN_UP;
                int8_t position, button;
                for (position = 0; position < N_POSITIONS; position = (position + 2)){
                    printf("floor %d : ", position);
                    for (button = 0; button < N_BUTTONS; button++){
                        printf("%d - ", queue_array[button][position]);
                    }
                    printf("\n");
                }
                fsm_state = IDLE;
                break;
            }
            elev_set_motor_direction(DIRN_UP);
            break;
            
        case IDLE:
            elev_set_motor_direction(DIRN_STOP);
            
            if (queue_is_queue_empty()) {
                break;
            }

            elev_motor_direction_t order_direction = queue_get_next_direction(fsm_position, fsm_direction);
            if (order_direction == DIRN_STOP) {
                fsm_timestamp = timer_start_timer();
                fsm_state = OPEN_DOOR;
                break;
            }
            elev_set_motor_direction(order_direction);
            fsm_previous_position = fsm_position;
            fsm_last_direction = fsm_direction;
            fsm_direction = order_direction;
            fsm_state = MOVING;
            //////////////// TO BE DELETED
            if (fsm_last_direction != fsm_direction) {
                printf("direction change from %s", fsm_print_direction(fsm_last_direction));
                printf("  to direction %s", fsm_print_direction(fsm_direction));
                printf("\n");
            }
            //////////////// TO BE DELETED
            break;

        case MOVING:
            if (fsm_previous_position == fsm_position) {
                break;
            }
            switch (fsm_position){
                case FLOOR_0:
                case FLOOR_1:
                case FLOOR_2:
                case FLOOR_3:
                    if (queue_should_stop(fsm_position, fsm_direction)) {
                        elev_set_motor_direction(DIRN_STOP);
                        fsm_timestamp = timer_start_timer();
                        fsm_state = OPEN_DOOR;
                    }
                    fsm_previous_position = fsm_position;
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
                int8_t floor = fsm_position/2;
                queue_delete_order(floor);
                fsm_state = IDLE;
            }
            break;
        
        case EMERGENCY_STOP:
            queue_reset_queue();
            elev_set_motor_direction(DIRN_STOP);
            elev_set_stop_lamp(1);
            if (elev_get_floor_sensor_signal() != -1) {
                elev_set_door_open_lamp(1); 
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
        for (int floor = 0; floor < N_FLOORS; floor++) {
            elev_button_type_t button_signal = elev_get_button_signal(button, floor);
//            printf("m_register_order_press: button signal %d\n", button_signal);
            if (button_signal == 1) {
                printf("add_order called\n");
                queue_set_order(button, floor);
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
        elev_set_floor_indicator(floor_sensor);
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
        printf("state change from %s", fsm_print_position(fsm_previous_position));
        printf("  to state %s", fsm_print_position(fsm_position));
        printf("\n");
    }
    ///////////////////TO BE DELETED
}

static void m_reset_order_lights(){
    for (int8_t button = 0; button < N_BUTTONS; button++) {
        for (int8_t floor = 0; floor < N_FLOORS; floor++) {
            }
        }
    }

const char* fsm_print_position(position_t position){
    switch (position) {
        case FLOOR_0: return "FLOOR_0";
        case FLOOR_1: return "FLOOR_1";
        case FLOOR_2: return "FLOOR_2";
        case FLOOR_3: return "FLOOR_3";
        case BETWEEN_0_AND_1: return "BETWEEN_0_AND_1";
        case BETWEEN_1_AND_2: return "BETWEEN_1_AND_2";
        case BETWEEN_2_AND_3: return "BETWEEN_2_AND_3";
        case UNKNOWN: return "UNKNOWN";
    
        default:
            return 0;
            break;
    }
}

const char* fsm_print_direction(elev_motor_direction_t direction){
    switch (direction) {
        case DIRN_DOWN: return "DIRN_DOWN";
        case DIRN_UP: return "DIRN_UP";
        case DIRN_STOP: return "DIRN_STOP";
    
        default:
            return 0;
            break;
    }
}