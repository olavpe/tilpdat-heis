/**
 * @file
 * @brief This class that controls the main functions of the 
 * elevator. It is responsible for checking input signals 
 * in addtion to managing the state of the elevator. 
 */ 

#pragma once

#define N_POSITIONS 7


/**
  Position type to keep track of the position of the elevator.
*/
typedef enum {
    FLOOR_0, ///< Elevator at floor 0
    BETWEEN_0_AND_1, ///< Elevator between floor 0 and floor 1
    FLOOR_1, ///< Elevator at floor 1
    BETWEEN_1_AND_2, ///< Elevator between floor 1 and floor 2
    FLOOR_2, ///< Elevator at floor 2
    BETWEEN_2_AND_3, ///< Elevator between floor 2 and floor 3
    FLOOR_3, ///< Elevator at floor 3
    UNKNOWN ///< Unknown position only during initialization
} position_t;

/**
  Floor type to dissern the different floors of the elevator. Mainly used when refering queue_array in queue.h
*/
typedef enum {
    ORDER_FLOOR_0, ///< Floor 0
    ORDER_FLOOR_1, ///< Floor 1
    ORDER_FLOOR_2, ///< Floor 2
    ORDER_FLOOR_3, ///< Floor 3
    ORDER_UNKNOWN ///< Unknown floor only during initialization
} floor_t;

/**
  State types for function fsm(). 
*/
typedef enum {
    INIT, ///< Initialization state, only during start up
    IDLE, ///< Idle state, where the elevator is not moving and checks for new orders.
    MOVING,  ///< Moving either up or down
    OPEN_DOOR, ///< Open door state where the elevator is not moving and opens the door.
    EMERGENCY_STOP  ///< Emergency stop state, regardless of position. 
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
 * The state machine manages the states in the ::state_t.
 * The behaviour between the states is described in the state diagram and the sequence diagram.
 * 
 * @param[out] fsm_position Elevator position
 * @param[out] fsm_floor Elevator floor
 * @param[out] fsm_timestamp Timestamp
 * @param[out] fsm_direction Elevator direction of travel. This will only ever by DIRN_UP or DIRN_DOWN of ::elev_motor_direction_t.
 * @param[out] fsm_state Elevator state
 */
void fsm();