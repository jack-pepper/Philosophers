/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/31 23:52:08 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// INSTEAD OF GETTING TIMESTAMP several times, add time_to_eat / time_to_sleep
// to the previous timestamp?
void	*philo_routine(void *arg)
{
	t_philo_arg	*this_arg;
	int			i;
	int			next_i;

	this_arg = (t_philo_arg *)arg;
	i = this_arg->i;
	set_next_i(this_arg->state, i, &next_i);
	wait_sim_start(&(*this_arg->state));
	gandalf_barrier(&(*this_arg->state));
	if (DEBUG == 1)
		printf("	🚀 👴 philo %d set, starting routine!\n", i + 1);
	if (this_arg->state->settings.number_of_philosophers == 1)
		endcase_die_alone(this_arg->state,
			&(*this_arg->state).philosophers[i], i);
	else
		have_council(&(*this_arg->state), i, next_i);
	return (0);
}

int	have_council(t_state *state, int i, int next_i)
{
	uint64_t	timestamp_ms;

	timestamp_ms = 0;
	pthread_mutex_lock(&(state)->mtx_sim_state);
	while (state->simulation_on == true)
	{
		pthread_mutex_unlock(&(state)->mtx_sim_state);
		if (wait_forks(state, timestamp_ms, i, next_i) != 0
			|| eat_pasta(state, timestamp_ms, i, next_i) != 0
			|| take_a_nap(state, timestamp_ms, i) != 0
			|| think(state, timestamp_ms, i) != 0)
		{
			endcase_grief(state, &(state)->philosophers[i], i);
			return (0);
		}
		pthread_mutex_lock(&(state)->mtx_sim_state);
	}
	return (0);
}

/*
int	wait_forks(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	if (is_sim_on(state) != 0)
		return (1);
	take_left_fork(state, i);
	if (is_sim_on(state) != 0)
	{
		put_left_fork(state, i);
		return (1);
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);

	take_right_fork(state, i, next_i);
	if (is_sim_on(state) != 0)
	{
		put_right_fork(state, i, next_i);
		put_left_fork(state, i);
		return (1);
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	return (0);
}
*/

int	wait_forks(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	if (is_sim_on(state) != 0)
		return (1);
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
		return (1);
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	take_right_fork(state, i, next_i);
	if (is_sim_on(state) != 0)
	{
		put_right_fork(state, i, next_i);
		put_left_fork(state, i);
		return (1);
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
		return (1);
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	take_left_fork(state, i);
	if (is_sim_on(state) != 0)
	{
		put_left_fork(state, i);
		put_right_fork(state, i, next_i);
		return (1);
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	return (0);
}
