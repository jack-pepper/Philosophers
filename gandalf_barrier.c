/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gandalf_barrier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:12:19 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/17 07:01:39 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	gandalf_barrier(t_state *state)
{
	if (DEBUG == 1)
		printf("\n🧙🚧 [gandalf_barrier]\n");
	ft_mutex_lock(&(state)->barrier.mtx_barrier);
	ft_mutex_lock(&(state)->mtx_threads_ready);
	state->threads_ready += 1;
	if (DEBUG == 1)
		printf("    🧵 threads_ready: ++ (%d)\n", state->threads_ready);
	if (state->threads_ready < state->barrier.threshold)
	{
		wait(state);
		return ;
	}
	else
	{
		break_through(state);
		if (DEBUG == 1)
			printf("\n	... 💥 Gandalf barrier destroyed!\n");
	}
	return ;
}

int	wait(t_state *state)
{
	ft_mutex_unlock(&(state)->mtx_threads_ready);
	ft_mutex_unlock(&(state)->barrier.mtx_barrier);
	ft_usleep(1000, "[gandalf_barrier] usleep failed\n");
	wait_philo_all_set(state);
	return (0);
}

int	wait_philo_all_set(t_state *state)
{
	ft_mutex_lock(&(state)->mtx_philo_all_set);
	while (state->philo_all_set == false)
	{
		ft_mutex_unlock(&(state)->mtx_philo_all_set);
		ft_usleep(1000, "[wait_philo_all_set] usleep failed\n");
		ft_mutex_lock(&(state)->mtx_philo_all_set);
		if (DEBUG == 1)
			printf("        🔒 mtx_philo_all_set: locked (FALSE: waiting...)\n");
	}
	ft_mutex_unlock(&(state)->mtx_philo_all_set);
	if (DEBUG == 1)
		printf("        🔓 mtx_philo_all_set: unlocked (is TRUE: let's go!)\n");
	return (0);
}

int	break_through(t_state *state)
{
	ft_mutex_unlock(&(state)->mtx_threads_ready);
	set_philo_threads_all_set(state, true);
	if (ft_mutex_destroy(&(state)->mtx_threads_ready,
			ERR_MTX_DEST_THREADS_READY) != 0)
		return (1);
	if (DEBUG == 1)
		printf("	philo_all_set set to TRUE!\n");
	ft_mutex_unlock(&(state)->barrier.mtx_barrier);
	ft_mutex_destroy(&(state)->barrier.mtx_barrier, ERR_MTX_DEST_BARRIER);
	return (0);
}
