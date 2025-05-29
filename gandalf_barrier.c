/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gandalf_barrier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:12:19 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/29 13:52:46 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	gandalf_barrier(t_state *state)
{	
	if (DEBUG == 1)
		printf("\n🧙🚧 [gandalf_barrier]\n");
	pthread_mutex_lock(&(state->barrier.mtx_barrier));	
	pthread_mutex_lock(&(state->mtx_threads_ready));
	state->threads_ready += 1;
	if (DEBUG == 1)
		printf("    🧵 threads_ready: ++ (%d)\n", state->threads_ready);
	if (state->threads_ready < state->barrier.threshold)
	{
		pthread_mutex_unlock(&(state->mtx_threads_ready));
		pthread_mutex_unlock(&(state->barrier.mtx_barrier)); // can return err too?
		if (usleep(1000) != 0)
                        printf("[gandalf_barrier] usleep failed\n");	
		wait_philo_all_set(state);
		return ;
	}
	else
        {
		set_philo_threads_all_set(state, true);
         	if (DEBUG == 1)
			printf("	[gandalf barrier] philo_all_set set to TRUE!\n");
	        pthread_mutex_unlock(&(state->barrier.mtx_barrier)); // can return err too?
                pthread_mutex_destroy(&(state->barrier.mtx_barrier));
                if (DEBUG == 1)
                        printf("	💥 Gandalf barrier destroyed!\n");
        }
        return ;
}

int     wait_philo_all_set(t_state *state)
{
        pthread_mutex_lock(&(state->mtx_philo_all_set));
        while (state->philo_all_set == false)
        {
                pthread_mutex_unlock(&(state->mtx_philo_all_set));
                if (usleep(1000) != 0)
                        printf("[wait_philo_all_set] usleep failed\n");
                pthread_mutex_lock(&(state->mtx_philo_all_set));
                if (DEBUG == 1)
                        printf("        🔒 mtx_philo_all_set: locked (is FALSE: waiting...)\n");
        }
        pthread_mutex_unlock(&(state->mtx_philo_all_set));
        if (DEBUG == 1)
                printf("        🔓 mtx_philo_all_set: unlocked (is TRUE: let's go!)\n");
        return (0);
}

