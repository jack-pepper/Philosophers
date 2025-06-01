/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_algorithm.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/01 21:19:31 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_forks(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	if (is_sim_on(state) != 0)
		return (EXIT_GRIEF);
	else if (are_philo_all_fed_up(state) != 0)
		return (EXIT_SATIETY);
	if (state->philosophers[i].is_left_handed == true)
		left_handed_case(state, timestamp_ms, i, next_i);
	else
		right_handed_case(state, timestamp_ms, i, next_i);
	return (0);
}

int	left_handed_case(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	take_left_fork(state, i);
	if (is_sim_on(state) != 0)
	{
		put_left_fork(state, i);
		return (EXIT_GRIEF);
	}
	else if (are_philo_all_fed_up(state) != 0)
	{
		put_left_fork(state, i);	
		return (EXIT_SATIETY);
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	take_right_fork(state, i, next_i);
	if (is_sim_on(state) != 0)
	{
		put_right_fork(state, i, next_i);
		put_left_fork(state, i);
		return (EXIT_GRIEF);
	}
	else if (are_philo_all_fed_up(state) != 0)
	{
		put_right_fork(state, i, next_i);
		put_left_fork(state, i);
		return (EXIT_SATIETY);
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	return (0);
}

int	right_handed_case(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	take_right_fork(state, i, next_i);
	if (is_sim_on(state) != 0)
	{
		put_right_fork(state, i, next_i);
		return (EXIT_GRIEF);
	}
	else if (are_philo_all_fed_up(state) != 0)
	{
		put_right_fork(state, i, next_i);
		return (EXIT_SATIETY);
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	take_left_fork(state, i);
	if (is_sim_on(state) != 0)
	{
		put_left_fork(state, i);
		put_right_fork(state, i, next_i);
		return (EXIT_GRIEF);
	}
	else if (are_philo_all_fed_up(state) != 0)
	{
		put_left_fork(state, i);
		put_right_fork(state, i, next_i);
		return (EXIT_SATIETY);
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	return (0);
}
