/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:15:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/30 10:04:56 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_left_fork(t_state *state, int i)
{
	int	ret;

	ret = 0;
	if (DEBUG == 1)
		printf("	⏳ 👴 philo %d waiting for fork 🍴 %d...\n",
			i + 1, i + 1);
	ret = pthread_mutex_lock(&(state)->forks[i].mtx_fork);
	if (ret != 0)
		return (1);
	change_fork_status(state, i, true);
	change_has_fork(state, i, "left", true);
	if (DEBUG == 1)
		printf("	🔒 👴 philo %d locked fork 🍴 %d!\n",
			i + 1, i + 1);
	return (ret);
}

int	take_right_fork(t_state *state, int i, int next_i)
{
	int	ret;

	ret = 0;
	if (DEBUG == 1)
		printf("	⏳ 👴 philo %d waiting for fork 🍴 %d...\n",
			i + 1, next_i + 1);
	ret = pthread_mutex_lock(&(state)->forks[next_i].mtx_fork);
	if (ret != 0)
		return (1);
	change_fork_status(state, i, true);
	change_has_fork(state, i, "right", true);
	if (DEBUG == 1)
		printf("	🔒 👴 philo %d locked fork 🍴 %d!\n",
			i + 1, next_i + 1);
	return (ret);
}

int	put_left_fork(t_state *state, int i)
{
	int	ret;

	ret = 0;
	change_fork_status(state, i, false);	
	change_has_fork(state, i, "left", false);
	pthread_mutex_unlock(&(state)->forks[i].mtx_fork);
	if (ret != 0)
		return (1);
	if (DEBUG == 1)
		printf("        🔓 👴 philo %d unlocked fork 🍴 %d!\n",
			i + 1, i + 1);
	return (ret);
}

int	put_right_fork(t_state *state, int i, int next_i)
{
	int	ret;

	ret = 0;
	change_fork_status(state, next_i, false);
	change_has_fork(state, i, "right", false);	
	pthread_mutex_unlock(&(state)->forks[next_i].mtx_fork);
	if (DEBUG == 1)
		printf("        🔓 👴 philo %d unlocked fork 🍴 %d!\n",
			i + 1, next_i + 1);
	return (ret);
}

int	change_fork_status(t_state *state, int i, bool is_taken)
{
	pthread_mutex_lock(&(state)->forks[i].mtx_is_taken);
	(state)->forks[i].is_already_taken = is_taken;
	pthread_mutex_unlock(&(state)->forks[i].mtx_is_taken);
	return (0);
}
