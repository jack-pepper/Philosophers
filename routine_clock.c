/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_clock.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/08 22:29:08 by mmalie           ###   ########.fr       */
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
	toll_the_bell(state);
	return (NULL);
}

void	toll_the_bell(t_state *state)
{
	volatile uint64_t	now_time;

	ft_mutex_lock(&(state->mtx_sim_state));
	while (state->simulation_on == true)
	{
		ft_mutex_unlock(&(state->mtx_sim_state));
		while (are_philo_threads_all_set(state) != 0)
			ft_usleep(1000, "[wait sim_start] usleep failed\n");
		if (state->settings.number_of_times_each_philosopher_must_eat > 0
			&& verify_satiety(state) == true)
			return ;
		now_time = get_cur_time(state);
		if (take_pulse(state, now_time) != 0)
		{
			if (DEBUG == 1)
			{
				printf("\n🔔 👻 ⏳ Philosophers, settle your paradoxes...\n");
				printf("	It's time to leave.\n");
			}
			return ;
		}
		ft_usleep(1000, "[toll_the_bell] usleep failed\n");
		ft_mutex_lock(&(state->mtx_sim_state));
	}
	return ;
}

// Called by the clock routine to check if any of the philosophers died
int	take_pulse(t_state *state, uint64_t timestamp_ms)
{
	int		i;
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

bool	verify_satiety(t_state *state)
{
	int	i;
	int	satiety;
	int	nb_guests;
	int	nb_meals;

	satiety = state->settings.number_of_times_each_philosopher_must_eat;
	if (satiety < 1)
		return (false);
	nb_guests = state->settings.number_of_philosophers;
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
