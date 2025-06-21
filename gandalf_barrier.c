/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gandalf_barrier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:12:19 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/21 22:34:02 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	gandalf_barrier(t_state *state)
{
	int	res;

	if (DEBUG == 1)
		printf("\n🧙🚧 [gandalf_barrier]\n");
	ft_mutex_lock(&(state)->barrier.mtx_barrier);
	ft_mutex_lock(&(state)->mtx_threads_ready);
	state->threads_ready += 1;
	if (DEBUG == 1)
		printf("    🧵 threads_ready: ++ (%d)\n", state->threads_ready);
	if (state->threads_ready < state->barrier.threshold)
	{
		res = wait(state);
		if (res == 1)
			return (1);
	}
	else
	{
		if (break_through(state) != 0)
			return (1);
		if (DEBUG == 1)
			printf("\n	... 💥 Gandalf barrier destroyed!\n");
		set_all_last_meal_time(state, state->settings.number_of_philosophers);
	}
	return (0);
}

int	wait(t_state *state)
{
	int	res;

	res = 0;
	ft_mutex_unlock(&(state)->mtx_threads_ready);
	ft_mutex_unlock(&(state)->barrier.mtx_barrier);
	ft_usleep(1000, "[gandalf_barrier] usleep failed\n");
	res = wait_philo_all_set(state);
	return (res);
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

void	set_all_last_meal_time(t_state *state, int nb_guests)
{
	int			i;
	uint64_t	now_time;

	i = 0;
	now_time = get_cur_time(state);
	ft_mutex_lock(&(state->clock.mtx_get_time));
	while (i < nb_guests)
	{
		state->philosophers[i].last_meal_time_ms = now_time;
		i++;
	}
	ft_mutex_unlock(&(state->clock.mtx_get_time));
	return ;
}
