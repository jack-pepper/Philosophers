/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_change.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/07 23:43:36 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	change_status(t_state *state, uint64_t timestamp_ms,
		t_philosopher *philosopher, char *status)
{
	int	res;

	ft_mutex_lock(&state->mtx_display_status);
	printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
	if (ft_strcmp(status, DIED_MSG) == 0)
	{
		die(state, philosopher);
		return (EXIT_GRIEF);
	}
	ft_mutex_unlock(&state->mtx_display_status);
	if (ft_strcmp(status, EAT_MSG) == 0)
	{
		res = eat(state, philosopher, timestamp_ms) != 0;
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
	if (state->settings.number_of_philosophers > 1)
		drop_forks(state, philosopher, philosopher->id - 1);
	if (DEBUG == 1)
	{
		printf("\n🥀 ⚰️  philo %d died of starvation!\n", philosopher->id);
		printf("	...\n");
	}
	philosopher->is_alive = false;
	ft_mutex_unlock(&state->mtx_sim_state);
	ft_mutex_unlock(&state->mtx_display_status);
	pthread_mutex_destroy(&state->mtx_display_status);
	return ;
}

int	eat(t_state *state, t_philosopher *philosopher, uint64_t timestamp_ms)
{
	int	satiety;
	int	meals_eaten;

	satiety = state->settings.number_of_times_each_philosopher_must_eat;
	ft_mutex_lock(&(state->clock.mtx_get_time));
	philosopher->last_meal_time_ms = timestamp_ms;
	if (satiety > 0)
	{
		ft_mutex_lock(&(philosopher->mtx_meals));
		meals_eaten = philosopher->meals_eaten += 1;
		ft_mutex_unlock(&(philosopher->mtx_meals));
		if (meals_eaten >= satiety
			&& verify_satiety(state) == true)
		{
			endcase_satiety(state, philosopher,
				philosopher->id - 1);
			return (EXIT_SATIETY);
		}
	}
	ft_mutex_unlock(&(state->clock.mtx_get_time));
	ft_usleep((int)state->settings.time_to_eat * 1000,
		"[change_status] usleep failed\n");
	return (0);
}

void	nap(t_state *state)
{
	ft_usleep((int)state->settings.time_to_sleep * 1000,
		"[change_status] usleep failed\n");
}
