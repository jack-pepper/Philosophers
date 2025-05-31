/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/31 10:41:53 by mmalie           ###   ########.fr       */
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
	uint64_t	timestamp_ms;

	timestamp_ms = 0;
	this_arg = (t_philo_arg *)arg;
	i = this_arg->i;
	
	set_next_i(this_arg->state, i, &next_i);
	wait_sim_start(&(*this_arg->state));
	gandalf_barrier(&(*this_arg->state));
	if (DEBUG == 1)	
		printf("	🚀 👴 philo %d set, starting routine!\n", i + 1);
	pthread_mutex_lock(&(this_arg->state)->mtx_sim_state);	
	while ((*this_arg->state).simulation_on == true)
	{
		pthread_mutex_unlock(&(this_arg->state)->mtx_sim_state);
		
		if (wait_forks((this_arg->state), timestamp_ms, i, next_i) != 0
			|| eat_pasta((this_arg->state), timestamp_ms, i, next_i) != 0
			|| take_a_nap((this_arg->state), timestamp_ms, i) != 0
			|| think((this_arg->state), timestamp_ms, i) != 0)
		{
			endcase_grief(&(*this_arg->state), &(*this_arg->state).philosophers[i], i);
			return (0);
		}
		pthread_mutex_lock(&(this_arg->state)->mtx_sim_state);
	}
	return (0);
}

uint64_t	calc_timestamp_ms(t_state *state, int i)
{
	uint64_t	timestamp_ms;

	pthread_mutex_lock(&(state->clock.mtx_get_time));
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	pthread_mutex_unlock(&(state->clock.mtx_get_time));	
	return (timestamp_ms);
}

int	wait_forks(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
//	int	ret;

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

int	eat_pasta(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
//	int	ret;

	if (is_sim_on(state) != 0)
		return (-1);
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status((state), timestamp_ms, &(state)->philosophers[i], EAT_MSG);
	put_right_fork(state, i, next_i);
	put_left_fork(state, i);
	return (0);
}

int	take_a_nap(t_state *state, uint64_t timestamp_ms, int i)
{	
	if (is_sim_on(state) != 0)
		return (-1);
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status((state), timestamp_ms, &(state)->philosophers[i], SLEEP_MSG);
	return (0);
}

int	think(t_state *state, uint64_t timestamp_ms, int i)
{
	if (is_sim_on(state) != 0)
		return (-1);
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status((state), timestamp_ms, &(state)->philosophers[i], THINK_MSG);
	return (0);
}
