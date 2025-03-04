#include "philo.h"

// Set start time and start the clock
int     set_clock(t_state *state)
{
    if (gettimeofday(&state->clock.start_time, NULL) != 0)
        printf("[set_clock] gettimeofday fail\n");
        state->clock.start_time_ms = state->clock.start_time.tv_sec / 1000;
    if (pthread_create(state->clock.thread, NULL, &clock_routine, &state) != 0)
    return (-1);
    if (phtread_join(state->clock.thread, NULL) != 0)
    return (-1);
    return (0);
}

// Constantly set current time and check if a philosopher has starved to death
void    *clock_routine(t_state *state)
{
    while (1)
    {
        if (gettimeofday(&state->clock.cur_time, NULL) != 0)
            printf("[clock_routine] gettimeofday fail\n");
        state->clock.cur_time_ms = state->clock.cur_time.tv_sec / 1000;
        take_pulse(state);
    }
}

// Called by the clock routine to check if any of the philosophers died
int	    take_pulse(t_state *state)
{
	int	i;
	int	starving_since;

	while (i < state->settings.number_of_philosophers)
	{
		starving_since = state->clock.cur_time_ms - state->philosophers[i].last_meal_time_ms;
		if (starving_since > state->settings.time_to_die)
		{
			change_status(&state->philosophers[i], "died");
			return (1);
		}
		i++;
	}
}