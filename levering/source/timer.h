/**
 * @file
 * @brief A smaller module that manages the time dependent operations of the state machine.
 * 
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>


/**
 * @brief Starts a fictious timer by returning timestamp of the current time of the system.
 * 
 * @return the start time.
 */
time_t timer_start_timer();

/**
 * @brief Check whether 3 seconds have passed 
 * since the timer started.
 *
 * @param[in] start_time Start time of timer.
 * 
 * @return 1 if the timer is expired, 0 if not.
 */
bool timer_is_timer_expired(time_t start_timestamp);

#endif