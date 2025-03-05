#include "philo.h"

// Set start time and start the clock
int     set_clock(t_state *state)
{
    state->clock.simulation_on = false;
    if (gettimeofday(&state->clock.start_time, NULL) != 0)
        printf("[set_clock] gettimeofday fail\n");
    printf("[set clock] %ld\n", state->clock.start_time.tv_sec);
    state->clock.start_time_ms = state->clock.start_time.tv_sec / 1000;
    printf("[set clock] %llu\n", state->clock.start_time_ms);
    if (pthread_create(&state->clock.thread, NULL, &clock_routine, &state) != 0)
        return (-1);
    if (pthread_join(state->clock.thread, NULL) != 0)
        return (-1);
    return (0);
}

// Constantly set current time and check if a philosopher has starved to death
void    *clock_routine(void *arg)
{
    t_state *state;
    
    printf("clock routine: thread launched!\n");
    state = (t_state *)arg;
    while (state->clock.simulation_on == false)
    {
        printf("Clock waiting...\n");
        if (usleep(1000) != 0)
            printf("[clock_routine] usleep failed\n");
    }
    while (1)
    {
        if (gettimeofday(&state->clock.cur_time, NULL) != 0)
            printf("[clock_routine] gettimeofday fail\n");
        state->clock.cur_time_ms = state->clock.cur_time.tv_sec * 1000 + state->clock.cur_time.tv_usec / 1000;
        printf("Current time (ms): %llu\n", state->clock.cur_time_ms);
        if (take_pulse(state) != 0)
        {
            on_exit(state);
        }
        if (usleep(1000) != 0)
            printf("[clock_routine] usleep failed\n");
    }
    return (0);
}

// Called by the clock routine to check if any of the philosophers died
int	    take_pulse(t_state *state)
{
	int	i;
	int	starving_since;

    i = 0;
	while (i < state->settings.number_of_philosophers)
	{
		starving_since = state->clock.cur_time_ms - state->philosophers[i].last_meal_time_ms;
        printf("philosopher %d starving since %llu...\n", i, state->philosophers[i].last_meal_time_ms);
		if (starving_since > state->settings.time_to_die)
		{
			change_status(&state->philosophers[i], "died");
			return (1);
		}
		i++;
	}
    return (0);
}

uint64_t    convert_to_ms()
{
    //
}