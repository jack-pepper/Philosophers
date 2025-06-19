/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_clock.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/19 21:58:25 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Constantly set current time and check if a philosopher has starved to death
void	*clock_routine(void *arg)
{
	t_state		*state;

	state = (t_state *)arg;
	if (DEBUG == 1)
		printf("\n🕰️ [clock_routine] Starting routine...\n");
	set_sim_status(state, true);
	while (are_philo_threads_all_set(state) != 0)
	{
		ft_usleep(1000, "[clock_routine] usleep failed\n");
		if (is_sim_on(state) == false)
		{
			if (DEBUG == 1)
				printf("\n ⏳ Error with philo threads: stopping clock...\n");
			return (NULL);
		}
	}
	toll_the_bell(state);
	return (NULL);
}

void	toll_the_bell(t_state *state)
{
	volatile uint64_t		now_time;
	int						satiety;

	satiety = state->settings.number_of_times_each_philosopher_must_eat;
	ft_mutex_lock(&(state->mtx_sim_state));
	while (state->simulation_on == true)
	{
		ft_mutex_unlock(&(state->mtx_sim_state));
		if (state->settings.number_of_times_each_philosopher_must_eat > 0
			&& verify_satiety(state, state->settings.number_of_philosophers,
				satiety) == true)
			return ;
		now_time = get_cur_time(state);
		if (take_pulse(state, now_time) != 0)
			return ;
		ft_usleep(1000, "[toll_the_bell] usleep failed\n");
		ft_mutex_lock(&(state->mtx_sim_state));
	}
	return ;
}

// Called by the clock routine to check if any of the philosophers died
int	take_pulse(t_state *state, uint64_t timestamp_ms)
{
	int			i;
	uint64_t	starving_since;

	i = 0;
	while (i < state->settings.number_of_philosophers)
	{
		starving_since = calc_starvation_duration(state, i);
		if (starving_since > (uint64_t)state->settings.time_to_die)
		{
			set_sim_status(state, false);
			change_status(state, timestamp_ms,
				&state->philosophers[i], "died");
			if (DEBUG == 1)
			{
				printf("\n🔔 👻 ⏳ Philosophers, settle your paradoxes...\n");
				printf("	It's time to leave.\n");
			}
			return (1);
		}
		i++;
	}
	return (0);
}

uint64_t	calc_starvation_duration(t_state *state, int i)
{
	uint64_t	starving_since;

	pthread_mutex_lock(&(state->clock.mtx_get_time));
	starving_since = state->clock.cur_time_ms
		- state->philosophers[i].last_meal_time_ms;
	pthread_mutex_unlock(&(state->clock.mtx_get_time));
	return (starving_since);
}

bool	verify_satiety(t_state *state, int nb_guests, int satiety)
{
	int	i;
	int	nb_meals;

	if (satiety < 1)
		return (false);
	i = 0;
	while (i < nb_guests)
	{
		ft_mutex_lock(&(state->philosophers[i].mtx_meals));
		nb_meals = state->philosophers[i].meals_eaten;
		ft_mutex_unlock(&(state->philosophers[i].mtx_meals));
		if (nb_meals < satiety)
			return (false);
		i++;
	}
	ft_mutex_lock(&state->mtx_philo_all_fed_up);
	state->philo_all_fed_up = true;
	ft_mutex_unlock(&state->mtx_philo_all_fed_up);
	return (true);
}
