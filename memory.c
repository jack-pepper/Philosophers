/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:33:19 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/29 00:25:25 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_on_exit(t_state *state)
{
	int	ret;

	ret = 0;
	ret = detach_threads(state);
	if (ret != 0)
		return (ret);
	free_philosophers(state);
	ret = free_forks(state);
	if (ret != 0)
		return (ret);
       // pthread_mutex_unlock(&state->mutex_display_status); // Locked previously by dead philo
	return (0);
}

int	detach_threads(t_state *state)
{
	int	res;
	int	i;

	res = 0;
	i = 0;
	while (i < state->settings.number_of_philosophers)
	{
		if (pthread_detach(state->philosophers[i].thread) != 0)
			return (-1); // how to free the philosophers and forks in case of error here?
		if (DEBUG == 1)
			printf("philo thread %d detached.\n", i + 1);
		i++;
	}
	if (pthread_detach(state->clock.thread) != 0)
		res++;
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
		// unlock not needed because of thread being detached already?
		/*pthread_mutex_lock(&(state)->forks[i].mtx_status);
		if ((state)->forks[i].is_already_taken == true)
		{
			// WRONG: each thread should unlock their own locks
			printf("FREE_FORKS: (status fork %d: %d)\n", i, (state)->forks[i].is_already_taken);
			pthread_mutex_unlock(&(state)->forks[i].mtx_status);
			pthread_mutex_unlock(&(state)->forks[i].mutex); // save value for error management
		}
		else
			pthread_mutex_unlock(&(state)->forks[i].mtx_status);
		if (pthread_mutex_destroy(&state->forks[i].mutex) != 0)
			res++;	
		if (pthread_mutex_destroy(&state->forks[i].mtx_status) != 0)
			res++;
		i++; */
		
		if (pthread_mutex_destroy(&state->forks[i].mtx_fork) != 0)
			res++;
		if (pthread_mutex_destroy(&state->forks[i].mtx_is_taken) != 0)
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

/*
void	destroy_all_mutexes(pthread_mutex_t mutex, int i)
{
	while (i > 0)
	{
		if (pthread_mutex_destroy(&state->forks[i].mutex) != 0)
			printf("❌ err: forks[%d].mtx_status not destroyed!\n", i);
		i--;
	}
}*/

int     clean_all_forks_mutexes(pthread_mutex_t *mtx, int i)
{
        while (i > 0)
        {
		i--;
                if (pthread_mutex_destroy(&mtx[i]) != 0)
                        return (ft_ret(1, "❌ err: mutex forks[%d] not destroyed!\n", i));
        }
        return (0);
}

