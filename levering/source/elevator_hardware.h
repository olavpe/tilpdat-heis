/**
 * @file
 * @brief The driver that communicates with the elevator hardware.
 * This is done through a server that is set up on a port
 * on the machine and communicates with the elevator hardware. 
 * The function interactions are identical to the driver provided
 * by the lab instructor in the Embedded Systems course.
 */ 

#pragma once

// Number of floors. Hardware-dependent, do not modify.
#define N_FLOORS 4  ///< Number of floors in ::floor_t without ORDER_FLOOR_UNKNOWN, also Hardware-dependent

// Number of buttons (and corresponding lamps) on a per-floor basis
#define N_BUTTONS 3 ///< Number of button types in ::elev_button_type_t

/**
  Motor direction for function elev_set_motor_direction().
*/
typedef enum { 
    DIRN_DOWN = -1, ///< Elevator motor direction downwards
    DIRN_STOP = 0, ///< Elevator motor stopped
    DIRN_UP = 1 ///< Elevator motor direction upwards
} elev_motor_direction_t;

/**
  Button types for function elev_set_button_lamp() and elev_get_button().
*/
typedef enum { 
    BUTTON_CALL_UP = 0, ///< Elevator hall order in upwards direction
    BUTTON_CALL_DOWN = 1,  ///< Elevator hall order in downwards direction
    BUTTON_COMMAND = 2  ///< Elevator cab order from within the elevator
} elev_button_type_t;

/**
  Initialize elevator.
  @return Non-zero on success, 0 on failure.
*/
void elev_init();

/**
  Sets the motor direction of the elevator.
  @param[in] dirn New direction of the elevator.
*/
void elev_set_motor_direction(elev_motor_direction_t dirn);

/**
  Set a button lamp.
  @param[in] lamp Which type of lamp to set as defined in ::elev_button_type_t.
  @param[in] floor Floor of lamp to set. Must be 0-3
  @param[in] value Non-zero value turns lamp on, 0 turns lamp off.
*/
void elev_set_button_lamp(elev_button_type_t button, int floor, int value);

/**
  Set floor indicator lamp for a given floor.
  @param[in] floor Which floor lamp to turn on. Other floor lamps are turned off.
*/
void elev_set_floor_indicator(int floor);

/**
  Turn door-open lamp on or off.
  @param[in] value Non-zero value turns lamp on, 0 turns lamp off.
*/
void elev_set_door_open_lamp(int value);

/**
  Turn stop lamp on or off.
  @param[in] value Non-zero value turns lamp on, 0 turns lamp off.
*/
void elev_set_stop_lamp(int value);

/**
  Gets a button signal.
  @param[in] button Which button type to check as defined in ::elev_button_type_t. 
  @param[in] floor Which floor to check button. Must be 0-3.
  @return 0 if button is not pushed. 1 if button is pushed.
*/
int elev_get_button_signal(elev_button_type_t button, int floor);

/**
  Get floor sensor signal.
  @return -1 if elevator is not on a floor. 0-3 if elevator is on floor. 0 is
    ground floor, 3 is top floor.
*/
int elev_get_floor_sensor_signal(void);

/**
  Get signal from stop button.
  @return 1 if stop button is pushed, 0 if not.
*/
int elev_get_stop_signal(void);

/**
  Get signal from obstruction switch.
  @return 1 if obstruction is enabled. 0 if not.
*/
int elev_get_obstruction_signal(void);