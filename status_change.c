/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_change.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/31 00:57:41 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	change_status(t_state *state, uint64_t timestamp_ms, t_philosopher *philosopher, char *status)
{
	pthread_mutex_lock(&state->mtx_display_status);
	printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
	if (ft_strcmp(status, DIED_MSG) == 0)
	{
		die(state, philosopher);
		return ;
	}
	pthread_mutex_unlock(&state->mtx_display_status);
	if (ft_strcmp(status, EAT_MSG) == 0)
		eat(state, philosopher, timestamp_ms);
	else if (ft_strcmp(status, SLEEP_MSG) == 0)
		nap(state);
	else
		return ;
}

void	die(t_state *state, t_philosopher *philosopher)
{
	drop_forks_in_agony(state, philosopher, philosopher->id - 1);
	pthread_mutex_unlock(&state->mtx_display_status);
	pthread_mutex_destroy(&state->mtx_display_status);
	free(philosopher->arg);
	return ;
}

void	eat(t_state *state, t_philosopher *philosopher, uint64_t timestamp_ms)
{
	pthread_mutex_lock(&(state->clock.mtx_get_time));
	philosopher->last_meal_time_ms = timestamp_ms;
	if (state->settings.number_of_times_each_philosopher_must_eat > 0)
	{
		philosopher->meals_eaten += 1;
		if (philosopher->meals_eaten >= state->settings.number_of_times_each_philosopher_must_eat
			&& verify_satiety(state) == true)
		{
				// [TODO] Stop sim if all are full
				;
		}
	}
	pthread_mutex_unlock(&(state->clock.mtx_get_time));
	ft_usleep((int)state->settings.time_to_eat * 1000, "[change_status] usleep failed\n");
	return ;
}

void	nap(t_state *state)
{
	ft_usleep((int)state->settings.time_to_sleep * 1000, "[change_status] usleep failed\n");
}
