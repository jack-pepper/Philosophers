/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:10:35 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/27 11:47:29 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	display_settings(const t_settings *settings)
{
	printf("⚙️ [DISPLAY_SETTINGS]⚙️\n");
	printf("number of philosophers: `%d`\n",
		settings->number_of_philosophers);
	printf("time to die: `%d`\n",
		settings->time_to_die);
	printf("time to eat: `%d`\n",
		settings->time_to_eat);
	printf("time to sleep: `%d`\n",
		settings->time_to_sleep);
	printf("number of times each philosopher must eat: `%d`\n",
		settings->number_of_times_each_philosopher_must_eat);
	return ;
}

uint64_t	get_timestamp_ms(struct timeval *tv)
{
	if (gettimeofday(tv, NULL) != 0)
		printf("[get_timestamp_ms] gettimeofday fail\n");
	return (convert_to_ms(*tv));
}

// static inline?
uint64_t	convert_to_ms(struct timeval time)
{
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_ret(int return_val, char *error_msg)
{
	printf("%s", error_msg);
	return (return_val);
}

void	gandalf_barrier(t_state *state)
{	
	pthread_mutex_lock(&(state->barrier.mtx_barrier));
	state->threads_ready += 1;
	if (DEBUG == 1)
		printf("🧙 [gandalf_barrier] threads_ready++: %d\n", state->threads_ready);
	if (state->threads_ready < state->barrier.threshold)
	{
		pthread_mutex_unlock(&(state->barrier.mtx_barrier)); // can return err too?
		if (DEBUG == 1)
			printf("[gandalf_barrier] mtx_barrier: unlocked!\n");
		pthread_mutex_lock(&(state->mtx_sim_state));
		if (DEBUG == 1) printf("[gandalf_barrier] mtx_sim_state: locked!\n");
		while (!state->simulation_on)
		{
			pthread_mutex_unlock(&(state->mtx_sim_state));
			if (DEBUG == 1)
				printf("[gandalf_barrier] mtx_sim_state: unlocked!\n");
			if (usleep(1000) != 0)
				printf("[gandalf_barrier] usleep failed\n");
			pthread_mutex_lock(&(state->mtx_sim_state));
			if (DEBUG == 1)
				printf("[gandalf_barrier] mtx_sim_state: locked!\n");
		}
		pthread_mutex_unlock(&(state->mtx_sim_state));
		if (DEBUG == 1)
			printf("[gandalf_barrier] mtx_sim_state: unlocked!\n");
		return ;
	}
	else
	{
		pthread_mutex_lock(&(state->mtx_sim_state));
		state->philo_all_set = true;
		state->simulation_on = true;	
		pthread_mutex_unlock(&(state->mtx_sim_state));
		pthread_mutex_unlock(&(state->barrier.mtx_barrier)); // can return err too?
		pthread_mutex_destroy(&(state->barrier.mtx_barrier));
	}
	return ;
}
