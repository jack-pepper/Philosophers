/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtx_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 19:20:25 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/16 07:45:35 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_fork_mutexes(t_state *state, int nb_guests, int *i)
{
	while ((*i) < nb_guests)
	{
		if (pthread_mutex_init(
				&state->forks[(*i)].mtx_fork, NULL) != 0)
			return (clean_ret(state,
					ERR_MTX_INIT_FORK, i, 1));
		if (pthread_mutex_init(
				&state->forks[(*i)].mtx_is_taken, NULL) != 0)
			return (clean_ret(state,
					ERR_MTX_INIT_IS_TAKEN, i, 2));
		if (pthread_mutex_init(
				&state->philosophers[(*i)].mtx_has_left_fork, NULL) != 0)
			return (clean_ret(state,
					ERR_MTX_INIT_HAS_LEFT_FORK, i, 3));
		if (pthread_mutex_init(
				&state->philosophers[(*i)].mtx_has_right_fork, NULL) != 0)
			return (clean_ret(state,
					ERR_MTX_INIT_HAS_RIGHT_FORK, i, 4));
		(*i)++;
	}
	return (0);
}

int	init_meals_mutexes(t_state *state, int nb_guests, int *i)
{
	while ((*i) < nb_guests)
	{
		if (pthread_mutex_init(
				&state->philosophers[(*i)].mtx_meals, NULL) != 0)
			return (clean_ret(state,
					ERR_MTX_INIT_MEALS, i, 5));
		(*i)++;
	}
	return (0);
}

int	clean_ret(t_state *state, char *err_msg, int *i, int lvl)
{
	ft_putstr_fd(err_msg, STDERR);
	(*i)--;
	if (lvl > 0)
		clean_all_forks_mutexes(&state->forks[(*i)].mtx_fork, i);
	if (lvl > 1)
		clean_all_forks_mutexes(&state->forks[(*i)].mtx_is_taken, i);
	if (lvl > 2)
		clean_all_forks_mutexes(
			&state->philosophers[(*i)].mtx_has_left_fork, i);
	if (lvl > 3)
		clean_all_forks_mutexes(
			&state->philosophers[(*i)].mtx_has_right_fork, i);
	if (lvl > 4)
		destroy_mutex(&state->mtx_display_status,
			ERR_MTX_DEST_DISPLAY_STATUS);
	if (lvl > 5)
		clean_all_forks_mutexes(&state->philosophers[(*i)].mtx_meals, i);
	if (lvl > 6)
		destroy_mutex(&state->clock.mtx_get_time,
			ERR_MTX_DEST_GET_TIME);
	if (lvl > 7)
		destroy_mutex(&state->mtx_sim_state,
			ERR_MTX_DEST_SIM_STATE);
	if (lvl > 8)
		destroy_mutex(&state->mtx_threads_ready,
			ERR_MTX_DEST_THREADS_READY);
	return (1);
}

int	clean_all_forks_mutexes(pthread_mutex_t *mtx, int *i)
{
	// include the err_msg, join the msg with the ft_itoa(number) + \n,
	//then free, to display the full msg
	while ((*i) > 0)
	{
		(*i)--;
		if (destroy_mutex(&mtx[(*i)], ERR_MTX_DEST_FORK) != 0)
			return (1);
		//if (pthread_mutex_destroy(&mtx[i]) != 0)
		//        return (ft_ret(1, "❌ err: mutex forks[%d] not destroyed!\n", i));
		if (destroy_mutex(&mtx[(*i)], ERR_MTX_DEST_MEALS) != 0)
			return (1);
	}
	return (0);
}
