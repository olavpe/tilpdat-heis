/**
 * @file
 * @brief This class that controls the main functions of the 
 * elevator. It is responsible for checking input signals 
 * in addtion to managing the state of the elevator. 
 */ 

#pragma once

#define N_POSITIONS 7


/**
  Position type to keep track of the position of the elevator. Used in functions
    - FILL IN FOLLOWING
*/
typedef enum {
    FLOOR_0,
    BETWEEN_0_AND_1,    
    FLOOR_1,
    BETWEEN_1_AND_2,
    FLOOR_2,
    BETWEEN_2_AND_3,
    FLOOR_3,
    UNKNOWN
} position_t;

/**
  Floor type to dissern the different floors of the elevator. Used in functions
    - FILL IN FOLLOWING
*/
typedef enum {
    ORDER_FLOOR_0,
    ORDER_FLOOR_1,
    ORDER_FLOOR_2,
    ORDER_FLOOR_3,
    ORDER_UNKNOWN
} floor_t;

/**
  State types for function fsm(). 
*/
typedef enum {
    INIT,
    IDLE,
    MOVING, 
    OPEN_DOOR,
    EMERGENCY_STOP 
} state_t; 


// functions
/**
 * @brief The functions main responsibilites is managing
 * the finite state machine. Before entering a switch that manages
 * the state machine it polls:
 *   - The order buttons
 *   - floor sensors
 *   - stop button
 * 
 * The state machine consists of the following states:
 *   - INIT
 *   - IDLE
 *   - MOVING
 *   - OPEN DOOR
 *   - EMERGENCY STOP
 * 
 * @param[out] fsm_position Elevator position
 * @param[out] fsm_last_position Elevator position
 * @param[out] fsm_timestamp Timestamp
 * @param[out] fsm_direction Elevator direction of travel
 * @param[out] fsm_state Elevator state
 */
void fsm();

///////////////////TO BE DELETED
 /**
  * @brief This function matches the number of the elevator position_t
  * enum to the corresponding string in the enum, allowing the string 
  * to be printed rather than the number
  * 
  * @param[in] position 
 */
const char* fsm_print_position(position_t position);

 /**
  * @brief This function matches the number of the elevator position_t
  * enum to the corresponding string in the enum, allowing the string 
  * to be printed rather than the number
 */
const char* fsm_print_direction(elev_motor_direction_t direction);
///////////////////TO BE DELETED
