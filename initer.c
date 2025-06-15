/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 19:20:25 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/15 21:37:37 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_barrier(t_state *state)
{
	if (pthread_mutex_init(&(state->barrier.mtx_barrier), NULL) != 0)
		return (1);
	state->barrier.threshold = state->settings.number_of_philosophers;
	if (DEBUG == 1)
	{
		printf(SUC_INIT_BARRIER);
		printf("Barrier's threshold set to %d\n",
			state->barrier.threshold);
	}
	return (0);
}

int	init_forks(t_state *state, int nb_guests)
{
	int	i;

	if (DEBUG == 1)
		printf("\n🐣🍴 init forks for %d guests...\n", nb_guests);
	state->forks = malloc(sizeof(t_fork) * nb_guests);
	if (!state->forks)
		return (1);
	memset(state->forks, 0, sizeof(t_fork) * nb_guests);
	i = 0;
	while (i < nb_guests)
	{
		state->forks[i].id = i + 1;
		state->forks[i].is_already_taken = false;
		i++;
	}
	if (DEBUG == 1)
		printf(SUC_INIT_FORKS);
	return (0);
}

int	init_philosophers(t_state *state, int nb_guests)
{
	int	i;

	if (DEBUG == 1)
		printf("\n🐣👴 init %d philosophers...\n", nb_guests);
	state->philosophers = malloc(sizeof(t_philosopher) * (nb_guests));
	if (!state->philosophers)
		return (1);
	memset(state->philosophers, 0, sizeof(t_philosopher) * nb_guests);
	i = 0;
	while (i < nb_guests)
	{
		state->philosophers[i].id = i + 1;
		state->philosophers[i].is_alive = true;
		state->philosophers[i].last_meal_time_ms = 0;
		state->philosophers[i].has_left_fork = false;
		state->philosophers[i].has_right_fork = false;
		i++;
	}
	if (DEBUG == 1)
		printf(SUC_INIT_PHILOSOPHERS);
	return (0);
}

int	init_mutexes(t_state *state, int nb_guests)
{
	int	i;

	if (DEBUG == 1)
		printf("\n🐣🔐 init mutexes...\n");
	i = 0;
	if (init_fork_mutexes(state, nb_guests, &i) != 0)
		return (1);
	if (pthread_mutex_init(&state->mtx_display_status, NULL) != 0)
		return (clean_ret(state, ERR_MTX_INIT_DISPLAY_STATUS, &i, 6));
	if (pthread_mutex_init(&state->clock.mtx_get_time, NULL) != 0)
		return (clean_ret(state, ERR_MTX_INIT_GET_TIME, &i, 7));
	if (pthread_mutex_init(&state->mtx_sim_state, NULL) != 0)
		return (clean_ret(state, ERR_MTX_INIT_SIM_STATE, &i, 8));
	if (pthread_mutex_init(&state->mtx_threads_ready, NULL) != 0)
		return (clean_ret(state, ERR_MTX_INIT_THREADS_READY, &i, 9));
	if (pthread_mutex_init(&state->mtx_philo_all_set, NULL) != 0)
		return (clean_ret(state, ERR_MTX_INIT_PHILO_ALL_SET, &i, 10));
	if (pthread_mutex_init(&state->mtx_philo_all_fed_up, NULL) != 0)
		return (clean_ret(state, ERR_MTX_INIT_PHILO_ALL_FED_UP, &i, 11));
	if (DEBUG == 1)
		printf(SUC_INIT_MUTEXES);
	return (0);
}
