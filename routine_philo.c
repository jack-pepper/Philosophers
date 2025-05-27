/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/27 23:42:04 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// INSTEAD OF GETTING TIMESTAMP several times, add time_to_eat / time_to_sleep
// to the previous timestamp?
void	*philo_routine(void *arg)
{
	t_philo_arg	*this_arg;
	int			i;
	int			next_i;
	uint64_t	timestamp_ms;

	timestamp_ms = 0;
	this_arg = (t_philo_arg *)arg;
	i = this_arg->i;
	if (i == this_arg->nb_guests - 1)
		next_i = 0;
	else
		next_i = i + 1;
	
	gandalf_barrier(&(*this_arg->state));

	if (DEBUG == 1)	
		printf("[philo_routine] philosopher %d set, starting routine!\n", i + 1);

	pthread_mutex_lock(&(this_arg->state)->mtx_sim_state);	
	while ((*this_arg->state).simulation_on == true)
	{
		pthread_mutex_unlock(&(this_arg->state)->mtx_sim_state);
		wait_forks((this_arg->state), timestamp_ms, i, next_i); // check return value
		eat_pasta((this_arg->state), timestamp_ms, i, next_i); // check return value
		take_a_nap((this_arg->state), timestamp_ms, i);
		think((this_arg->state), timestamp_ms, i);
		pthread_mutex_lock(&(this_arg->state)->mtx_sim_state);
	}
	return (0);
}

int	wait_forks(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	int	ret;

	if (DEBUG == 1)
		printf("⏳ 👴 philo %d waiting for fork 🍴 %d...\n", i + 1, i + 1);	
	ret = pthread_mutex_lock(&(state)->forks[i].mutex);
	if (DEBUG == 1)	
		printf("🔒 👴 philo %d locked fork 🍴 %d!\n", i + 1, i + 1);	
	if (ret != 0)
		return (-1);
	(state)->forks[i].is_already_taken = true;
		
	pthread_mutex_lock(&(state->clock.mtx_get_time));

	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	pthread_mutex_unlock(&(state->clock.mtx_get_time));	
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	
	if (DEBUG == 1)
		printf("⏳ 👴 philo %d waiting for fork 🍴 %d...\n", i + 1, next_i + 1);	
	ret = pthread_mutex_lock(&(state)->forks[next_i].mutex);
	if (ret != 0)
		return (-1);
	if (DEBUG == 1)	
		printf("🔒 👴 philo %d locked fork 🍴 %d!\n", i + 1, next_i + 1);	
	
	(state)->forks[next_i].is_already_taken = true;
	pthread_mutex_lock(&(state->clock.mtx_get_time));
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	pthread_mutex_unlock(&(state->clock.mtx_get_time));	
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	//printf("//philo %d took fork %d ...//\n", i + 1, next_i + 1);
	return (0);
}

int	eat_pasta(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	int	ret;

	pthread_mutex_lock(&(state->clock.mtx_get_time));
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	pthread_mutex_unlock(&(state->clock.mtx_get_time));

	change_status((state), timestamp_ms, &(state)->philosophers[i], EAT_MSG);	


	(state)->forks[next_i].is_already_taken = false;
	ret = pthread_mutex_unlock(&(state)->forks[next_i].mutex);	
	if (ret != 0)
		return (-1);	


	(state)->forks[i].is_already_taken = false;
	//printf("philo %d put fork %d down...\n", i + 1, i + 1);
	ret = pthread_mutex_unlock(&(state)->forks[i].mutex);
	if (ret != 0)
		return (-1);

	return (0);
}

void	take_a_nap(t_state *state, uint64_t timestamp_ms, int i)
{	
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	change_status((state), timestamp_ms, &(state)->philosophers[i], SLEEP_MSG);
}

void	think(t_state *state, uint64_t timestamp_ms, int i)
{
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	change_status((state), timestamp_ms, &(state)->philosophers[i], THINK_MSG);
}

