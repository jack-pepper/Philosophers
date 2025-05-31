/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_clock.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/31 10:05:35 by mmalie           ###   ########.fr       */
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
			ft_usleep(1000, "[wait sim_start] usleep failed\n");
		now_time = get_cur_time(state);
		if (take_pulse(state, now_time) != 0)
		{
			if (DEBUG == 1)
				printf("🔔 👻 ⏳ Philosophers, settle your paradoxes: you are running out of time.\n");
			return (0);
		}	
		ft_usleep(1000, "[toll_the_bell] usleep failed\n");
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

/*void	drop_forks_in_agony(t_state *state, t_philosopher *philosopher, int i)
{
	int	next_i;

	if (i == state->settings.number_of_philosophers - 1)
		next_i = 0;
	else
		next_i = i + 1;

	pthread_mutex_lock(&(state)->philosophers[i].mtx_has_left_fork);
	if (philosopher->has_left_fork == true)
	{
		pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
		put_left_fork(state, philosopher->id - 1);
	}
	else
		pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
	pthread_mutex_lock(&(state)->philosophers[i].mtx_has_right_fork);
	if (philosopher->has_right_fork == true)
	{
		pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
		put_right_fork(state, philosopher->id - 1, next_i);
	}
	else
		pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
	return ;
}*/

bool	verify_satiety(t_state *state)
{
	int	i;
	int	satiety;
	int	nb_guests;

	satiety = state->settings.number_of_times_each_philosopher_must_eat;
	nb_guests = state->settings.number_of_philosophers;
	i = 0;
	while (i < nb_guests)
	{
		if (state->philosophers[i].meals_eaten < satiety)
			return (false);
		i++;
	}
	return (true);
}
