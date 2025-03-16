/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_clock.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/16 21:00:34 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Constantly set current time and check if a philosopher has starved to death
void	*clock_routine(void *arg)
{
	t_state	*state;

	state = (t_state *)arg;
	if (!state)
	{
		printf("Error: NULL state in clock_routine!\n");
		return (NULL);
	}
	printf("clock routine: thread launched!\n");
	//pthread_mutex_lock(&state->mutex_start_simulation);
	//printf("[clock_routine] state->clock.simulation_on = %d\n", state->simulation_on);
	//while (state->simulation_on == false)
	//{
	//    printf("Clock waiting...\n");
	//    if (usleep(1000) != 0)
	//    printf("[clock_routine] usleep failed\n");
	//}
	//printf("[clock_routine] state->clock.simulation_on = %d - Tic toc!\n", state->simulation_on);
	//pthread_mutex_unlock(&state->mutex_start_simulation);
	//pthread_mutex_destroy(&state->mutex_start_simulation); 
	while (state->philo_all_set == false)
	{
		printf("[clock_routine] waiting for all philos to be set: philo_all_set = %d\n", state->philo_all_set);
		if (usleep(1000) != 0)
                        printf("[clock_routine] usleep failed\n");
	}
	while (state->philo_all_set == true)//state->simulation_on == true)
	{	
		state->clock.cur_time_ms = get_timestamp_ms(&state->clock.cur_time) - state->clock.start_time_ms;
		//if (gettimeofday(&state->clock.cur_time, NULL) != 0)
		//	printf("[clock_routine] gettimeofday fail\n"); 
		//pthread_mutex_lock(&state->clock.mutex_get_time);
		//state->clock.cur_time_ms = convert_to_ms(state->clock.cur_time);
		//   printf("[clock_routine] Current time (ms): %lu\n", state->clock.cur_time_ms);
		// pthread_mutex_unlock(&state->clock.mutex_get_time);
		if (take_pulse(state, state->clock.cur_time_ms) != 0)
		{
			return (0);
		}
		if (usleep(1000) != 0)
			printf("[clock_routine] usleep failed\n");
	}
	return (0);
}

// Called by the clock routine to check if any of the philosophers died
int	take_pulse(t_state *state, uint64_t timestamp_ms)
{
	int	        i;
	uint64_t	starving_since;

	i = 0;
	while (i < state->settings.number_of_philosophers)
	{
		starving_since = state->clock.cur_time_ms - state->philosophers[i].last_meal_time_ms; // overflow
	//	printf("[take_pulse] clock.cur_time_ms: %lu - state->philosophers[i].last_meal_time_ms: %lu\n", state->clock.cur_time_ms, state->philosophers[i].last_meal_time_ms);
	//	printf("[take_pulse] philosopher %d starving since %lu...\n", i + 1, starving_since);
		if (starving_since > (uint64_t)state->settings.time_to_die)
		{	
			state->philo_all_set = false;
			state->simulation_on = false;
			change_status(state, timestamp_ms, &state->philosophers[i], "died");
			return (1);
		}
		i++;
	}
	return (0);
}

void	change_status(t_state *state, uint64_t timestamp_ms, t_philosopher *philosopher, char *status)
{
	if (ft_strncmp(status, DIED_MSG, ft_strlen(DIED_MSG)) == 0)
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
	}
	else if (ft_strncmp(status, FORK_MSG, ft_strlen(FORK_MSG)) == 0)
	{	
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
	}
	else if (ft_strncmp(status, EAT_MSG, ft_strlen(EAT_MSG)) == 0)
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		philosopher->last_meal_time_ms = timestamp_ms;
		usleep((int)state->settings.time_to_eat);
		//printf("%d has finished eating!\n", philosopher->id);
	}
	else if (ft_strncmp(status, SLEEP_MSG, ft_strlen(SLEEP_MSG)) == 0)
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		usleep((int)state->settings.time_to_sleep);
		//printf("%d woke up!\n", philosopher->id);
	}
	else if (ft_strncmp(status, THINK_MSG, ft_strlen(THINK_MSG)) == 0)
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

