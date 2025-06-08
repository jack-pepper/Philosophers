/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/08 23:49:26 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo_arg	*this_arg;
	int			res;
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
	{
		res = have_council(&(*this_arg->state), i, next_i);
		ft_mutex_lock(&(*this_arg->state).mtx_sim_state);
		if (this_arg->state->philosophers[i].is_alive == false)
		{
			ft_mutex_unlock(&(*this_arg->state).mtx_sim_state);
			endcase_agony(this_arg->state, &(this_arg->state)->philosophers[i], i);
			return (0);
		}
		ft_mutex_unlock(&(*this_arg->state).mtx_sim_state);
		if (res == EXIT_GRIEF)
			endcase_grief(this_arg->state, &(this_arg->state)->philosophers[i], i);
		else if (res == EXIT_SATIETY)
			endcase_satiety(this_arg->state, &(this_arg->state)->philosophers[i], i);
	}
	return (0);
}

int	set_next_i(t_state *state, int i, int *next_i)
{
	if (i == state->settings.number_of_philosophers - 1)
		(*next_i) = 0;
	else
		(*next_i) = i + 1;
	return ((*next_i));
}

int	wait_sim_start(t_state *state)
{
	ft_mutex_lock(&(state->mtx_sim_state));
	while (!state->simulation_on)
	{
		ft_mutex_unlock(&(state->mtx_sim_state));
		ft_usleep(1000, "[wait sim_start] usleep failed\n");
		ft_mutex_lock(&(state->mtx_sim_state));
		if (DEBUG == 1)
			printf("        🔒 mtx_sim_state: locked (simulation_on is FALSE: waiting...)\n");
	}
	ft_mutex_unlock(&(state->mtx_sim_state));
	if (DEBUG == 1)
		printf("        🔓 mtx_sim_state: unlocked (simulation_on is TRUE: let's go!)\n");
	return (0);
}

int	have_council(t_state *state, int i, int next_i)
{
	int		res;
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
