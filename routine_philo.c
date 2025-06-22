/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/22 21:39:53 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo_arg	*this_arg;
	int			i;
	int			next_i;

	this_arg = (t_philo_arg *)arg;
	i = this_arg->i;
	set_next_i(this_arg->state, i, &next_i);
	wait_sim_start(&(*this_arg->state));
	if (gandalf_barrier(&(*this_arg->state)) != 0)
	{
		printf("ERROR WHILE LAUNCHING PTHREAD!\n");
		free(this_arg);
		return (0);
	}
	if (i % 2 == 1)
		ft_usleep(this_arg->state, this_arg->state->settings.optimal_delay,
			"[philo_routine] usleep failed\n");
	if (DEBUG == 1)
		printf("	🚀 👴 philo %d set, starting routine!\n", i + 1);
	ft_mutex_lock(&(this_arg->state)->mtx_sim_state);
	if (this_arg->state->simulation_on == false)
	{
		ft_mutex_unlock(&(this_arg->state)->mtx_sim_state);
		endcase_grief(this_arg->state,
			&(this_arg->state)->philosophers[i], i); // ...
		return (0);
	}
	ft_mutex_unlock(&(this_arg->state)->mtx_sim_state);
	wait_for_death(this_arg, i, next_i);
	return (0);
}

int	wait_sim_start(t_state *state)
{
	ft_mutex_lock(&(state->mtx_sim_state));
	while (state->simulation_on == false)
	{
		ft_mutex_unlock(&(state->mtx_sim_state));
		ft_usleep(state, 1000, "[wait sim_start] usleep failed\n");
		ft_mutex_lock(&(state->mtx_sim_state));
		if (DEBUG == 1)
			printf("        🔒 mtx_sim_state: locked (sim OFF: waiting...)\n");
	}
	ft_mutex_unlock(&(state->mtx_sim_state));
	if (DEBUG == 1)
		printf("        🔓 mtx_sim_state: unlocked (sim ON: let's go!)\n");
	return (0);
}

void	wait_for_death(t_philo_arg *this_arg, int i, int next_i)
{
	int	res;

	if (this_arg->state->settings.number_of_philosophers == 1)
		endcase_die_alone(this_arg->state,
			&(*this_arg->state).philosophers[i], i);
	else
	{
		res = have_council(&(*this_arg->state), i, next_i);
		ft_mutex_lock(&(*this_arg->state).mtx_sim_state);
		if (this_arg->state->philosophers[i].is_alive == false)
		{
			ft_mutex_unlock(&(*this_arg->state).mtx_sim_state);
			endcase_agony(this_arg->state,
				&(this_arg->state)->philosophers[i]);
			return ;
		}
		ft_mutex_unlock(&(*this_arg->state).mtx_sim_state);
		if (res == EXIT_GRIEF)
			endcase_grief(this_arg->state,
				&(this_arg->state)->philosophers[i], i);
		else if (res == EXIT_SATIETY)
			endcase_satiety(this_arg->state,
				&(this_arg->state)->philosophers[i], i);
	}
	return ;
}

int	have_council(t_state *state, int i, int next_i)
{
	int			res;
	uint64_t	timestamp_ms;

	res = 0;
	timestamp_ms = 0;
	ft_mutex_lock(&(state)->mtx_sim_state);
	while (state->simulation_on == true)
	{
		ft_mutex_unlock(&(state)->mtx_sim_state);
		res = wait_forks(state, timestamp_ms, i, next_i);
		if (check_exit_case(res) != 0)
			return (res);
		res = eat_pasta(state, timestamp_ms, i, next_i);
		if (check_exit_case(res) != 0)
			return (res);
		res = take_a_nap(state, timestamp_ms, i);
		if (check_exit_case(res) != 0)
			return (res);
		res = think(state, timestamp_ms, i);
		if (check_exit_case(res) != 0)
			return (res);
		ft_mutex_lock(&(state)->mtx_sim_state);
	}
	ft_mutex_unlock(&(state)->mtx_sim_state);
	return (0);
}
