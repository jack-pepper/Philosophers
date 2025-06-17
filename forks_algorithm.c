/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_algorithm.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/17 07:07:50 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_endcase_met(t_state *state)
{
	if (is_sim_on(state) != 0)
		return (EXIT_GRIEF);
	else if (are_philo_all_fed_up(state) != 0)
		return (EXIT_SATIETY);
	return (0);
}

int	wait_forks(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	if (is_endcase_met(state) != 0)
		return (is_endcase_met(state));
	if (next_i == 0)
		return (left_handed_case(state, timestamp_ms, i, next_i));
	else
	{
		take_right_fork(state, i, next_i);
		if (is_endcase_met(state) != 0)
		{
			put_right_fork(state, i, next_i);
			return (is_endcase_met(state));
		}
		timestamp_ms = calc_timestamp_ms(state, i);
		change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
		take_left_fork(state, i);
		if (is_endcase_met(state) != 0)
		{
			put_left_fork(state, i);
			put_right_fork(state, i, next_i);
			return (is_endcase_met(state));
		}
		timestamp_ms = calc_timestamp_ms(state, i);
		change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	}
	return (0);
}

int	left_handed_case(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	take_left_fork(state, i);
	if (is_endcase_met(state) != 0)
	{
		put_left_fork(state, i);
		return (is_endcase_met(state));
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	take_right_fork(state, i, next_i);
	if (is_endcase_met(state) != 0)
	{
		put_right_fork(state, i, next_i);
		put_left_fork(state, i);
		return (is_endcase_met(state));
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	return (0);
}

void	drop_forks(t_state *state, t_philosopher *philosopher, int i)
{
	int	next_i;

	next_i = set_next_i(state, i, &next_i);
	ft_mutex_lock(&(state)->philosophers[i].mtx_has_left_fork);
	if (philosopher->has_left_fork == true)
	{
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
		put_left_fork(state, philosopher->id - 1);
	}
	else
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
	ft_mutex_lock(&(state)->philosophers[i].mtx_has_right_fork);
	if (philosopher->has_right_fork == true)
	{
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
		put_right_fork(state, philosopher->id - 1, next_i);
	}
	else
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
	return ;
}

void	change_has_fork(t_state *state, int i, char *side, bool has_fork)
{
	if (ft_strcmp(side, "left") == 0)
	{
		ft_mutex_lock(&(state)->philosophers[i].mtx_has_left_fork);
		state->philosophers[i].has_left_fork = has_fork;
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
	}
	else if (ft_strcmp(side, "right") == 0)
	{
		ft_mutex_lock(&(state)->philosophers[i].mtx_has_right_fork);
		state->philosophers[i].has_right_fork = has_fork;
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
	}
}
