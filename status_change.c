/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_change.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/25 11:40:42 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_dead_spotted(t_state *state)
{
	ft_mutex_lock(&state->mtx_sim_state);
	if (state->dead_spotted == false)
	{
		ft_mutex_unlock(&state->mtx_sim_state);
		return (false);
	}
	else
	{
		ft_mutex_unlock(&state->mtx_sim_state);
		return (true);
	}
}

void	set_last_meal_time(t_state *state, t_philosopher *philosopher, uint64_t timestamp_ms)
{
	ft_mutex_lock(&(state->clock.mtx_get_time));
	philosopher->last_meal_time_ms = timestamp_ms;
	ft_mutex_unlock(&(state->clock.mtx_get_time));
}

int	change_status(t_state *state, uint64_t timestamp_ms,
		t_philosopher *philosopher, char *status)
{
	int	res;

	ft_mutex_lock(&state->mtx_display_status);
	timestamp_ms = calc_timestamp_ms(state, philosopher->id - 1);
	if (is_dead_spotted(state) == false)
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
	else
	{
		ft_mutex_unlock(&state->mtx_display_status);
		return (0);
	}
	if (ft_strcmp(status, DIED_MSG) == 0)
	{
		die(state, philosopher);
		return (EXIT_GRIEF);
	}
	ft_mutex_unlock(&state->mtx_display_status);
	if (is_sim_on(state) == false)
		return (0);
	if (ft_strcmp(status, EAT_MSG) == 0)
	{
		set_last_meal_time(state, philosopher, timestamp_ms);
		res = eat(state, philosopher) != 0;
		if (res == EXIT_SATIETY)
			return (EXIT_SATIETY);
	}
	else if (ft_strcmp(status, SLEEP_MSG) == 0)
		nap(state);
	return (0);
}

void	die(t_state *state, t_philosopher *philosopher)
{
	if (DEBUG == 1)
		printf("	🍽️ 😩 [change_status] last_meal: %ld\n",
			philosopher->last_meal_time_ms);
	ft_mutex_lock(&state->mtx_sim_state);
	if (DEBUG == 1)
	{
		printf("\n🥀 ⚰️  philo %d died of starvation!\n", philosopher->id);
		printf("	...\n");
	}
	philosopher->is_alive = false;
	state->dead_spotted = true;
	ft_mutex_unlock(&state->mtx_sim_state);
	ft_mutex_unlock(&state->mtx_display_status);
	return ;
}

int	eat(t_state *state, t_philosopher *philosopher)
{
	int	satiety;

	satiety = state->settings.number_of_times_each_philosopher_must_eat;
	ft_usleep(state, (int)state->settings.time_to_eat * 1000,
		"[change_status] usleep failed\n");
	if (satiety > 0)
	{
		ft_mutex_lock(&(philosopher->mtx_meals));
		philosopher->meals_eaten += 1;
		ft_mutex_unlock(&(philosopher->mtx_meals));
	}
	return (0);
}

void	nap(t_state *state)
{
	ft_usleep(state, (int)state->settings.time_to_sleep * 1000,
		"[change_status] usleep failed\n");
}
