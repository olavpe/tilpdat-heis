/**
 * @file
 * @brief ...
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/**
 * @brief Sets a timestamp that is the current time
 * in seconds since 1 Jan 1970.
 * 
 * @return the current time.
 */
time_t timer_set_timestamp(void);

/**
 * @brief Starts the timer by setting a timestamp.
 * 
 * @return the start time.
 */
time_t timer_start_timer();

/**
 * @brief Check whether it has been 3 seconds
 * since the timer started.
 *
 * @param[in] start_time Start time of timer.
 * 
 * @return 1 if the timer is expired, 0 if not.
 */
bool timer_is_timer_expired(time_t start_time);

#endif