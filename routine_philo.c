/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/29 00:01:59 by mmalie           ###   ########.fr       */
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
		if (wait_forks((this_arg->state), timestamp_ms, i, next_i) != 0)
			break ;
		if (eat_pasta((this_arg->state), timestamp_ms, i, next_i) != 0)
			break ; // check return value
		if (take_a_nap((this_arg->state), timestamp_ms, i) != 0)
			break ;
		if (think((this_arg->state), timestamp_ms, i) != 0)
			break ;
		pthread_mutex_lock(&(this_arg->state)->mtx_sim_state);
	}
	return (0);
}

int	wait_forks(t_state *state, uint64_t timestamp_ms, int i, int next_i)
{
	int	ret;

	if (is_sim_on(state) != 0)
		return (-1);
	if (DEBUG == 1)
		printf("⏳ 👴 philo %d waiting for fork 🍴 %d...\n", i + 1, i + 1);	
	ret = pthread_mutex_lock(&(state)->forks[i].mtx_fork);
	if (DEBUG == 1)	
		printf("🔒 👴 philo %d locked fork 🍴 %d!\n", i + 1, i + 1);	
	if (ret != 0)
		return (-1);
	if (is_sim_on(state) != 0)
	{
		pthread_mutex_unlock(&(state)->forks[i].mtx_fork);
		return (-1);
	}
	
	pthread_mutex_lock(&(state)->forks[i].mtx_is_taken);
	(state)->forks[i].is_already_taken = true;	
	pthread_mutex_unlock(&(state)->forks[i].mtx_is_taken);

	pthread_mutex_lock(&(state->clock.mtx_get_time));
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	pthread_mutex_unlock(&(state->clock.mtx_get_time));	
	
	change_status(state, timestamp_ms, &(state)->philosophers[i], FORK_MSG);
	
	if (DEBUG == 1)
		printf("⏳ 👴 philo %d waiting for fork 🍴 %d...\n", i + 1, next_i + 1);	
	ret = pthread_mutex_lock(&(state)->forks[next_i].mtx_fork);
	if (ret != 0)
		return (-1);
	if (DEBUG == 1)	
		printf("🔒 👴 philo %d locked fork 🍴 %d!\n", i + 1, next_i + 1);
	if (is_sim_on(state) != 0)
	{
		pthread_mutex_unlock(&(state)->forks[next_i].mtx_fork);
		pthread_mutex_unlock(&(state)->forks[i].mtx_fork);
		return (-1);
	}
	
	pthread_mutex_lock(&(state)->forks[next_i].mtx_is_taken);
	(state)->forks[next_i].is_already_taken = true;
	pthread_mutex_unlock(&(state)->forks[next_i].mtx_is_taken);
	
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

	if (is_sim_on(state) != 0)
		return (-1);
	pthread_mutex_lock(&(state->clock.mtx_get_time));
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	pthread_mutex_unlock(&(state->clock.mtx_get_time));

	change_status((state), timestamp_ms, &(state)->philosophers[i], EAT_MSG);

	pthread_mutex_lock(&(state)->forks[next_i].mtx_is_taken);
	(state)->forks[next_i].is_already_taken = false;
	pthread_mutex_unlock(&(state)->forks[next_i].mtx_is_taken);

	ret = pthread_mutex_unlock(&(state)->forks[next_i].mtx_is_taken);
	if (ret != 0)
		return (-1);
	if (DEBUG == 1)
		printf("	🔓 👴 philo %d unlocked fork 🍴 %d!\n", i + 1, next_i + 1);	

	pthread_mutex_lock(&(state)->forks[i].mtx_is_taken);
	(state)->forks[i].is_already_taken = false;
	pthread_mutex_unlock(&(state)->forks[i].mtx_is_taken);

	ret = pthread_mutex_unlock(&(state)->forks[i].mtx_fork);
	if (ret != 0)
		return (-1);
	if (DEBUG == 1)
		printf("	🔓 👴 philo %d unlocked fork 🍴 %d!\n", i + 1, i + 1);	
	return (0);
}

int	take_a_nap(t_state *state, uint64_t timestamp_ms, int i)
{	
	if (is_sim_on(state) != 0)
		return (-1);
	pthread_mutex_lock(&(state->clock.mtx_get_time));
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;	
	pthread_mutex_unlock(&(state->clock.mtx_get_time));
	change_status((state), timestamp_ms, &(state)->philosophers[i], SLEEP_MSG);
	return (0);
}

int	think(t_state *state, uint64_t timestamp_ms, int i)
{
	if (is_sim_on(state) != 0)
		return (-1);
	pthread_mutex_lock(&(state->clock.mtx_get_time));
	timestamp_ms = get_timestamp_ms(&(state)->philosophers[i].cur_time) - (state)->clock.start_time_ms;
	pthread_mutex_unlock(&(state->clock.mtx_get_time));
	change_status((state), timestamp_ms, &(state)->philosophers[i], THINK_MSG);
	return (0);
}

