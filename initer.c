/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 19:20:25 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/29 10:49:47 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_barrier(t_state *state)
{
	if (pthread_mutex_init(&(state->barrier.mtx_barrier), NULL) != 0)
		return (1);	
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
		return (1);
	memset(state->forks, 0, sizeof(t_fork) * nb_guests);
	i = 0;
	while (i < nb_guests)
	{
		state->forks[i].id = i + 1;
		state->forks[i].is_already_taken = false;
		i++;
	}
	if (DEBUG == 1)
		printf("👍 forks ready!\n");
	return (0);
}

int	init_philosophers(t_state *state, int nb_guests)
{
	int	i;

	if (DEBUG == 1)
		printf("\n🐣👴 [init_philosophers] mallocating for %d philosophers...\n", nb_guests);
	state->philosophers = malloc(sizeof(t_philosopher) * (nb_guests));
	if (!state->philosophers)
		return (1);
	memset(state->philosophers, 0, sizeof(t_philosopher) * nb_guests);
	i = 0;
	while (i < nb_guests)
	{
		state->philosophers[i].id = i + 1;
                state->philosophers[i].last_meal_time_ms = 0;
		i++;
	}
	if (DEBUG == 1)
		printf("👍 philosophers ready!\n");
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
//	if (pthread_mutex_init(&state->clock.mtx_get_time, NULL) != 0)
//		return (ft_ret(-1, "❌ Issue while initiating mtx_get_time!\n", STDERR));
	i = 0;
	while (i < nb_guests)
	{
		if (pthread_mutex_init(&state->forks[i].mtx_fork, NULL) != 0)
		{
			printf("❌ Issue while initiating forks[%d] mutex\n", i);
			i--;
			clean_all_forks_mutexes(&state->forks[i].mtx_fork, i);
			//while (i > 0)
			//{
			//	if (pthread_mutex_destroy(&state->forks[i].mutex) != 0)
			//		printf("❌ err: forks[%d].mutex not destroyed!\n", i);
			//	i--;
		//	}
			return (1);
		}
		if (pthread_mutex_init(&state->forks[i].mtx_is_taken, NULL) != 0)
		{
			printf("❌ Issue while initiating forks[%d].mtx_status\n", i);
			i--;
			clean_all_forks_mutexes(&state->forks[i].mtx_is_taken, i);
//			while (i > 0)
//			{
//				if (pthread_mutex_destroy(&state->forks[i].mtx_status) != 0)
//					printf("❌ err: forks[%d].mtx_status not destroyed!\n", i);
//				i--;
//			}
			return (1);
		}
		i++;
	}

	if (pthread_mutex_init(&state->mtx_display_status, NULL) != 0)
	{
		clean_all_forks_mutexes(&state->forks[i].mtx_fork, i);
		return (ft_ret(1, "❌ Issue while initiating mtx_display_status\n", STDERR));
        }	
	if (pthread_mutex_init(&state->clock.mtx_get_time, NULL) != 0)
	{
		 if (pthread_mutex_destroy(&state->mtx_display_status) != 0)
			return (ft_ret(1, "❌ err: mtx_display_status not destroyed!\n", STDERR));
		clean_all_forks_mutexes(&state->forks[i].mtx_fork, i);
		return (ft_ret(1, "❌ Issue while initiating clock.mutex_get_time\n", STDERR));
        }
	if (pthread_mutex_init(&state->mtx_sim_state, NULL) != 0)
	{
		 if (pthread_mutex_destroy(&state->mtx_display_status) != 0)
			return (ft_ret(1, "❌ err: mtx_display_status not destroyed!\n", STDERR));
		clean_all_forks_mutexes(&state->forks[i].mtx_fork, i);
		return (ft_ret(1, "❌ Issue while initiating clock.mutex_get_time\n", STDERR));
        }	
	if (pthread_mutex_init(&state->mtx_threads_ready, NULL) != 0)
	{
		 if (pthread_mutex_destroy(&state->mtx_display_status) != 0)
			return (ft_ret(1, "❌ err: mtx_display_status not destroyed!\n", STDERR));
		clean_all_forks_mutexes(&state->forks[i].mtx_fork, i);
		return (ft_ret(1, "❌ Issue while initiating clock.mutex_get_time\n", STDERR));
        }
	if (pthread_mutex_init(&state->mtx_philo_all_set, NULL) != 0)
	{
		 if (pthread_mutex_destroy(&state->mtx_display_status) != 0)
			return (ft_ret(1, "❌ err: mtx_display_status not destroyed!\n", STDERR));
		clean_all_forks_mutexes(&state->forks[i].mtx_fork, i);
		return (ft_ret(1, "❌ Issue while initiating clock.mutex_get_time\n", STDERR));
        }
	if (DEBUG == 1)
		printf("👍 mutexes ready!\n");
	return (0);
}
