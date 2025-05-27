/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 19:20:25 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/28 00:00:38 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_barrier(t_state *state)
{
	if (pthread_mutex_init(&(state->barrier.mtx_barrier), NULL) != 0)
	{
		// free function
		return (-1);
	}
	
	state->barrier.threshold = state->settings.number_of_philosophers;
	return (0);
}

int	init_forks(t_state *state, int nb_guests)
{
	int	i;

	if (DEBUG == 1)
		printf("\n🐣🍴 [init_forks] mallocating forks for %d guests...\n", nb_guests);
	state->forks = malloc(sizeof(t_fork) * nb_guests);
	if (!state->forks)
		return (-1);
	memset(state->forks, 0, sizeof(t_fork) * nb_guests);
	if (DEBUG == 1)
	printf("	✅ forks successfully mallocated and initialized!\n");
	i = 0;
	while (i < nb_guests)
	{
		state->forks[i].id = i + 1;
		state->forks[i].is_already_taken = false;
		if (DEBUG == 1)
		{
			printf("	✅ fork %d set to %d (should be FALSE)!\n",
				state->forks[i].id, state->forks[i].is_already_taken);
		}
		i++;
	}
	if (DEBUG == 1)
		printf("	👍 forks ready!\n");
	return (0);
}

int	init_philosophers(t_state *state, int nb_guests)
{
	int	i;

	if (DEBUG == 1)
		printf("\n🐣👴 [init_philosophers] mallocating for %d philosophers...\n", nb_guests);
	state->philosophers = malloc(sizeof(t_philosopher) * (nb_guests));
	if (!state->philosophers)
		return (-1);
	memset(state->philosophers, 0, sizeof(t_philosopher) * nb_guests);
	if (DEBUG == 1)
		printf("	✅ philosophers successfully mallocated and initialized!\n");
	i = 0;
	while (i < nb_guests)
	{
		state->philosophers[i].id = i + 1;
                state->philosophers[i].last_meal_time_ms = 0;
		if (DEBUG == 1)
		{
			printf("	✅ philo %d last_meal_time_ms set to %ld \n",
				state->philosophers[i].id, state->philosophers[i].last_meal_time_ms);
		}
		i++;
	}
	if (DEBUG == 1)
		printf("	👍 philosophers ready!\n");
	return (0);
}

/*
int     init_clock(t_state *state)
{
        if (gettimeofday(&(*state)->clock.start_time, NULL) != 0)
                printf("[init_clock] gettimeofday fail\n");
        if (gettimeofday(&(*state)->clock.cur_time, NULL) != 0)
                printf("[init_clock] gettimeofday fail\n");
        state->clock.start_time_ms = convert_to_ms(state->clock.start_time);
        state->clock.cur_time_ms = convert_to_ms(state->clock.cur_time);
        printf("[init_clock] %lu\n", state->clock.start_time_ms);
        return (0);
}
*/

int	init_mutexes(t_state *state, int nb_guests)
{
	int	i;

	if (DEBUG == 1)
		printf("\n🐣🔐 [init_mutexes] initiating mutexes...\n");
	if (pthread_mutex_init(&state->clock.mtx_get_time, NULL) != 0)
	{
		printf("❌ Issue while initiating mtx_get_time!\n");
		return (-1);
	}
	i = 0;
	while (i < nb_guests)
	{
		if (pthread_mutex_init(&state->forks[i].mutex, NULL) != 0)
		{
			printf("❌ Issue while initiating forks[%d] mutex\n", i);
			i--;
			while (i > 0)
			{
				if (pthread_mutex_destroy(&state->forks[i].mutex) != 0)
					printf("❌ err: forks[%d].mutex not destroyed!\n", i);
				i--;
			}
			return (-1);
		}
		if (pthread_mutex_init(&state->forks[i].status, NULL) != 0)
		{
			printf("❌ Issue while initiating forks[%d].mtx_status\n", i);
			i--;


			//destroy_all_mutexes(&state->forks[i].mutex, i);

			while (i > 0)
			{
				if (pthread_mutex_destroy(&state->forks[i].status) != 0)
					printf("❌ err: forks[%d].mtx_status not destroyed!\n", i);
				i--;
			}
			return (-1);
		}




		i++;
	}
	if (DEBUG == 1)
		printf("	✅ forks successfully mutexed!\n");
	if (pthread_mutex_init(&state->mutex_display_status, NULL) != 0)
	{
		// free mutexes
		if (DEBUG == 1)
			printf("	❌ Issue while initiating mutex_display_status\n");
            	return (-1);
        }	
	if (pthread_mutex_init(&state->clock.mtx_get_time, NULL) != 0)
	{
		// free mutexes
		if (DEBUG == 1)
			printf("	❌ Issue while initiating clock.mutex_get_time\n");
            	return (-1);
        }
	if (DEBUG == 1)
		printf("	👍 mutexes ready!\n");
	return (0);
}
