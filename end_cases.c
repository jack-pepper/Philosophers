/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_cases.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 10:03:31 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/31 23:54:37 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	set_next_i(t_state *state, int i, int *next_i)
{
        if (i == state->settings.number_of_philosophers - 1)
                (*next_i) = 0;
        else
                (*next_i) = i + 1;
	return ((*next_i));
}

void	drop_forks(t_state *state, t_philosopher *philosopher, int i)
{
	int	next_i;

	next_i = set_next_i(state, i, &next_i);
        pthread_mutex_lock(&(state)->philosophers[i].mtx_has_left_fork);
        if (philosopher->has_left_fork == true)
        {
                pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
                put_left_fork(state, philosopher->id - 1);
        }
        else
                pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
        pthread_mutex_lock(&(state)->philosophers[i].mtx_has_right_fork);
        if (philosopher->has_right_fork == true)
        {
                pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
                put_right_fork(state, philosopher->id - 1, next_i);
        }
        else
                pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
        return ;
}

void	endcase_die_alone(t_state *state, t_philosopher *philosopher, int i)
{
	uint64_t	timestamp_ms;

	take_left_fork(state, i);
	if (is_sim_on(state) != 0)
	{
		put_left_fork(state, i);
		return ;
	}
	timestamp_ms = calc_timestamp_ms(state, i);
	change_status(state, timestamp_ms, philosopher, FORK_MSG);
	pthread_mutex_lock(&(state)->mtx_sim_state);
	while (state->simulation_on == true)
	{
		pthread_mutex_unlock(&(state)->mtx_sim_state);	
		ft_usleep(1000, "[endcase_die_alone] usleep failed\n");
		pthread_mutex_lock(&(state)->mtx_sim_state);
	}
	put_left_fork(state, i);
	return ;
}

void	endcase_agony(t_state *state, t_philosopher *philosopher, int i)
{
	drop_forks(state, philosopher, i);
	free(philosopher->arg);
	if (DEBUG == 1)
		printf("\n🥀 ⚰️  philo %d died of starvation! Something seems to exist but the individual's consciousness.\n", i + 1);
	return ;
}

void	endcase_grief(t_state *state, t_philosopher *philosopher, int i)
{
	drop_forks(state, philosopher, i);
	free(philosopher->arg);
	if (DEBUG == 1)
		printf("\n❤️🩹 👴 philo %d won't eat pasta out of ethical consideration.\n", i + 1);
	return ;
}

void	endcase_satiety(t_state *state, t_philosopher *philosopher, int i)
{
	drop_forks(state, philosopher, i);
	free(philosopher->arg);
	if (DEBUG == 1)
		printf("\n🍵🍃 👴 philo %d had %d meals, enough! There is measure in all things.\n", i + 1, philosopher->meals_eaten);
	return ;
}
