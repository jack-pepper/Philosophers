/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/31 21:34:17 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	calc_timestamp_ms(t_state *state, int i)
{
	uint64_t	timestamp_ms;

	pthread_mutex_lock(&(state->clock.mtx_get_time));
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	pthread_mutex_unlock(&(state->clock.mtx_get_time));	
	return (timestamp_ms);
}

int	eat_pasta(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
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
