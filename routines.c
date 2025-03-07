/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/07 23:29:50 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	*philo_routine(void *arg)
{
	t_philo_arg	*this_arg;
	int	i;
	int	next_i;
	
	this_arg = (t_philo_arg *)arg;
	i = this_arg->i;
	if (i == this_arg->nb_guests)
		next_i = 0;
	else
		next_i = i + 1;
	while (1)
	{
		// try to take 'left' fork (with same id as philosopher)
		//printf("Trying to take left fork...\n");
		pthread_mutex_lock(&this_arg->state->forks[i].mutex);
		change_status(this_arg->state, &this_arg->state->philosophers[i], FORK_MSG);
		this_arg->state->forks[i].is_already_taken = true;
		// try to take 'right' fork (i + 1, or 1 if last philosopher of the circle)
		//printf("Trying to take right fork...\n");
		pthread_mutex_lock(&this_arg->state->forks[i + 1].mutex);
		change_status(this_arg->state, &this_arg->state->philosophers[i], FORK_MSG);
		this_arg->state->forks[next_i].is_already_taken = true;
		// eat during the chosen time
		change_status(this_arg->state, &this_arg->state->philosophers[i], EAT_MSG);
	
		// unlock only after finishing eating
		//printf("Put right fork down\n");
		this_arg->state->forks[next_i].is_already_taken = false;
		pthread_mutex_unlock(&this_arg->state->forks[i + 1].mutex);
		//printf("Put left fork down\n");
		this_arg->state->forks[i].is_already_taken = false;
		pthread_mutex_unlock(&this_arg->state->forks[i].mutex);
	}
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
			change_status(state, &state->philosophers[i], "died");
			return (1);
		}
		i++;
	}
    return (0);
}

void	change_status(t_state *state, t_philosopher *philosopher, char *status)
{
	uint64_t timestamp_ms;

	timestamp_ms = state->clock.cur_time_ms;
	if (ft_strncmp(status, DIED_MSG, ft_strlen(DIED_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		// display message and end simulation
	}
	else if (ft_strncmp(status, FORK_MSG, ft_strlen(FORK_MSG)))
	{	
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		//
	}
	else if (ft_strncmp(status, EAT_MSG, ft_strlen(EAT_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		// launch a counter starting from the beginning of the meal
		// wait time_to_eat time
		// unlock the forks here?
	}
	else if (ft_strncmp(status, SLEEP_MSG, ft_strlen(SLEEP_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		//
	}
	else if (ft_strncmp(status, THINK_MSG, ft_strlen(THINK_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		/* code */
	}

	else
	{
		printf("[change_status] Wrong message!\n");
		return ;
	}
}

