/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_clock.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/29 14:32:52 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Constantly set current time and check if a philosopher has starved to death
void	*clock_routine(void *arg)
{
	t_state		*state;
	int		res;

	state = (t_state *)arg;
//	if (!state)
//		return (NULL);
	if (DEBUG == 1)
		printf("\n🕰️ [clock_routine] Starting routine...\n");	
	set_sim_status(state, true);
	res = toll_the_bell(state);
	if (res != 0)
		return (NULL);
	return (NULL);
}

int	toll_the_bell(t_state *state)
{
	volatile uint64_t	now_time;

	pthread_mutex_lock(&(state->mtx_sim_state));
	while (state->simulation_on == true)
	{
		pthread_mutex_unlock(&(state->mtx_sim_state));
		while (are_philo_threads_all_set(state) != 0)
		{
			if (usleep(1000) != 0)
                       		printf("[wait sim_start] usleep failed\n");
		}
		now_time = get_cur_time(state);
		if (take_pulse(state, now_time) != 0)
		{
			// unlock cur_time here? or in free_on_exit?
			if (usleep(5000) != 0) // should give time for other threads to stop properly
                       		printf("[wait sim_start] usleep failed\n");	
			free_on_exit(state);			
			return (0); // ???
		}	
		if (usleep(1000) != 0)
			printf("[toll_the_bell] usleep failed\n");
		pthread_mutex_lock(&(state->mtx_sim_state));
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
		starving_since = calc_starvation_duration(state, i);
		if (starving_since > (uint64_t)state->settings.time_to_die)
		{
			set_sim_status(state, false);
			change_status(state, timestamp_ms, &state->philosophers[i], "died");
			return (1);
		}
		i++;
	}
	return (0);
}

void	change_status(t_state *state, uint64_t timestamp_ms, t_philosopher *philosopher, char *status)
{
	pthread_mutex_lock(&state->mtx_display_status);
	printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
	if (ft_strcmp(status, DIED_MSG) == 0)
	{
		// Should put the forks if they have it at this point?
		pthread_mutex_unlock(&state->mtx_display_status);
		pthread_mutex_destroy(&state->mtx_display_status);
		return ;
	}
	pthread_mutex_unlock(&state->mtx_display_status);

	if (ft_strcmp(status, EAT_MSG) == 0)
	{
		pthread_mutex_lock(&(state->clock.mtx_get_time));
		philosopher->last_meal_time_ms = timestamp_ms;
		pthread_mutex_unlock(&(state->clock.mtx_get_time));
		usleep((int)state->settings.time_to_eat * 1000);
	}
	else if (ft_strcmp(status, SLEEP_MSG) == 0)
		usleep((int)state->settings.time_to_sleep * 1000);
	else
		return ;
}

