/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/11 23:17:21 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	calc_timestamp_ms(t_state *state, int i)
{
	uint64_t	timestamp_ms;

	ft_mutex_lock(&(state->clock.mtx_get_time));
	timestamp_ms = get_timestamp_ms(
			&(state)->philosophers[i].cur_time)
		- (state)->clock.start_time_ms;
	ft_mutex_unlock(&(state->clock.mtx_get_time));
	return (timestamp_ms);
}

int	check_exit_case(int res)
{
	if (res == EXIT_GRIEF)
		return (EXIT_GRIEF);
	else if (res == EXIT_SATIETY)
		return (EXIT_SATIETY);
	else
		return (0);
}

int	eat_pasta(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	int	res;

	res = 0;
	if (is_endcase_met(state) != 0)
		return (is_endcase_met(state));
	timestamp_ms = calc_timestamp_ms(state, i);
	ft_mutex_lock(&(state->clock.mtx_get_time));
        state->philosophers[i].last_meal_time_ms = timestamp_ms;
        ft_mutex_unlock(&(state->clock.mtx_get_time));
	res = change_status((state), timestamp_ms, &(state)->philosophers[i],
			EAT_MSG);
	if (res != 0)
		return (res);
	put_right_fork(state, i, next_i);
	put_left_fork(state, i);
	return (0);
}

int	take_a_nap(t_state *state, uint64_t timestamp_ms, int i)
{
	int	res;

	res = 0;
	if (is_endcase_met(state) != 0)
		return (is_endcase_met(state));
	timestamp_ms = calc_timestamp_ms(state, i);
	res = change_status((state), timestamp_ms, &(state)->philosophers[i],
			SLEEP_MSG);
	return (res);
}

int	think(t_state *state, uint64_t timestamp_ms, int i)
{
	int	res;

	res = 0;
	if (is_endcase_met(state) != 0)
		return (is_endcase_met(state));
	timestamp_ms = calc_timestamp_ms(state, i);
	res = change_status((state), timestamp_ms, &(state)->philosophers[i],
			THINK_MSG);
	return (res);
}
