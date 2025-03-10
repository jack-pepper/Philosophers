/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:33:19 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/10 14:09:42 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_on_exit(t_state *state)
{
	detach_threads(state); // get return value?
	free_forks(state);
	free_philosophers(state);
	return (0);
}

int	detach_threads(t_state *state)
{
	int	res;
	int	i;

	res = 0;
	if (pthread_detach(state->clock.thread) != 0)
	res++;
	i = 0;
	while (i < state->settings.number_of_philosophers)
	{
		if (pthread_detach(state->philosophers[i].thread) != 0)
			return (-1); // how to free the philosophers and forks in case of error here?
		i++;
	}
	if (res != 0)
	printf("Warning: %d threads could not be detached during cleanup.\n", res);
	printf("[detach_threads] %d threads successfully detached.\n", state->settings.number_of_philosophers - res); // DEBUG
	return (res);
}

int	free_forks(t_state *state)
{
	int	res;
	int	i;

	res = 0;
	i = 0;
	while (i < state->settings.number_of_philosophers)
	{
		if (pthread_mutex_destroy(&state->forks[i].mutex) != 0)
			res++;
		i++;
	}
	free(state->forks);
	if (res != 0)
	printf("Warning: %d mutex could not be destroyed during cleanup.\n", res);
	return (res);
}

void	free_philosophers(t_state *state)
{
    /*
    int i;

    i = 0;
    while (i < state->settings.number_of_philosophers)
    {
        free(&state->philosophers[i]);
        i++;
    }
    */
	free(state->philosophers);
}
