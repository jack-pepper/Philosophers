/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:28:51 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/21 23:17:50 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_start_time(t_state *state)
{
	pthread_mutex_lock(&(state->clock.mtx_get_time));
	state->clock.cur_time_ms = convert_to_ms(state->clock.cur_time);
	state->clock.start_time_ms = state->clock.cur_time_ms;
	if (DEBUG == 1)
		printf("        ⌚ cur_time_ms: %lu - start_time_ms: %lu\n",
			state->clock.cur_time_ms, state->clock.start_time_ms);
	pthread_mutex_unlock(&(state->clock.mtx_get_time));
	return ;
}

uint64_t	get_cur_time(t_state *state)
{
	volatile uint64_t	cur_time;

	ft_mutex_lock(&(state->clock.mtx_get_time));
	state->clock.cur_time_ms = get_timestamp_ms(&state->clock.cur_time)
		- state->clock.start_time_ms;
	cur_time = state->clock.cur_time_ms;
	ft_mutex_unlock(&(state->clock.mtx_get_time));
	return (cur_time);
}

uint64_t	get_timestamp_ms(struct timeval *tv)
{
	if (gettimeofday(tv, NULL) != 0)
		printf("[get_timestamp_ms] gettimeofday fail\n");
	return (convert_to_ms(*tv));
}

uint64_t	convert_to_ms(struct timeval time)
{
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_usleep(t_state *state, useconds_t usec, char *err_msg)
{
	uint64_t	start_time;
	uint64_t	ms;

	ms = usec / 1000;
	start_time = get_cur_time(state);
	while (get_cur_time(state) - start_time < ms)
	{
		if (usleep(100) != 0)
			ft_putstr_fd(err_msg, STDERR);
	}
	//if (usleep(usec) != 0)
	//	ft_putstr_fd(err_msg, STDERR);
}
