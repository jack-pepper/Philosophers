#include "philo.h"

// Set start time and start the clock
int     set_clock(t_state **state)
{
    if (gettimeofday(&(*state)->clock.start_time, NULL) != 0)
        printf("[set_clock] gettimeofday fail\n");
    if (gettimeofday(&(*state)->clock.cur_time, NULL) != 0)
        printf("[set_clock] gettimeofday fail\n");
    (*state)->clock.start_time_ms = convert_to_ms((*state)->clock.start_time);
    (*state)->clock.cur_time_ms = convert_to_ms((*state)->clock.cur_time);
    printf("[set clock] %lu\n", (*state)->clock.start_time_ms);
    if (pthread_create(&(*state)->clock.thread, NULL, &clock_routine, (*state)) != 0)
        return (-1);
    return (0);
}

// Constantly set current time and check if a philosopher has starved to death
void    *clock_routine(void *arg)
{
    t_state *state;
    
    state = (t_state *)arg;
    if (!state)
    {
        printf("Error: NULL state in clock_routine!\n");
        return (NULL);
    }
    printf("clock routine: thread launched!\n");
    pthread_mutex_lock(&state->mutex_start_simulation);
    printf("state->clock.simulation_on = %d\n", state->simulation_on);
    while (state->simulation_on == false)
    {
        printf("Clock waiting...\n");
        if (usleep(1000) != 0)
        printf("[clock_routine] usleep failed\n");
    }
    pthread_mutex_unlock(&state->mutex_start_simulation);
    pthread_mutex_destroy(&state->mutex_start_simulation);    
    while (1)
    {
        if (gettimeofday(&state->clock.cur_time, NULL) != 0)
        printf("[clock_routine] gettimeofday fail\n");
        state->clock.cur_time_ms = convert_to_ms(state->clock.cur_time);
        printf("Current time (ms): %lu\n", state->clock.cur_time_ms);
        if (take_pulse(state) != 0)
        {
            free_on_exit(state);
        }
        if (usleep(1000) != 0)
            printf("[clock_routine] usleep failed\n");
    }
    return (0);
}

// Called by the clock routine to check if any of the philosophers died
int	    take_pulse(t_state *state)
{
	int	        i;
	uint64_t	starving_since;

    i = 0;
	while (i < state->settings.number_of_philosophers)
	{
		starving_since = state->clock.cur_time_ms - state->philosophers[i].last_meal_time_ms;
        printf("clock.cur_time_ms: %lu - state->philosophers[i].last_meal_time_ms: %lu\n", state->clock.cur_time_ms, state->philosophers[i].last_meal_time_ms);
        printf("philosopher %d starving since %lu...\n", i, starving_since);
		if (starving_since > (uint64_t)state->settings.time_to_die)
		{
			change_status(&state, &state->philosophers[i], "died");
			return (1);
		}
		i++;
	}
    return (0);
}

uint64_t    convert_to_ms(struct timeval time)
{
    uint64_t    converted;

    converted = time.tv_sec * 1000 + time.tv_usec / 1000; 
    return (converted);
}
