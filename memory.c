/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:33:19 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/25 18:33:02 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_on_exit(t_state *state, int ret_val)
{
	int	ret;

	ret = 0;
	pthread_mutex_destroy(&state->mtx_display_status);
	free_philosophers(state);
	ret = free_forks(state);
	if (ret != 0)
		return (ret);
	return (ret_val);
}

int	free_forks(t_state *state)
{
	int	res;
	int	i;

	res = 0;
	i = 0;
	while (i < state->settings.number_of_philosophers)
	{
		if (pthread_mutex_destroy(&(state)->forks[i].mtx_fork) != 0)
			res++;
		if (pthread_mutex_destroy(&(state)->forks[i].mtx_is_taken) != 0)
			res++;
		i++;
	}
	if (state->forks)
		free(state->forks);
	return (res);
}

void	free_philosophers(t_state *state)
{
	if (state->philosophers)
		free(state->philosophers);
}

void	free_philo_args(t_state *state, int i)
{
	free(state->philosophers[i].arg);
	i--;
	while (i < 0)
		free(state->philosophers[i++].arg);
	return ;
}
