/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_cases.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 10:03:31 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/01 21:44:30 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	drop_forks(t_state *state, t_philosopher *philosopher, int i)
{
	int	next_i;

	next_i = set_next_i(state, i, &next_i);
	ft_mutex_lock(&(state)->philosophers[i].mtx_has_left_fork);
	if (philosopher->has_left_fork == true)
	{
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
		put_left_fork(state, philosopher->id - 1);
	}
	else
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
	ft_mutex_lock(&(state)->philosophers[i].mtx_has_right_fork);
	if (philosopher->has_right_fork == true)
	{
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
		put_right_fork(state, philosopher->id - 1, next_i);
	}
	else
		ft_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
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
	ft_mutex_lock(&(state)->mtx_sim_state);
	while (state->simulation_on == true)
	{
		ft_mutex_unlock(&(state)->mtx_sim_state);
		ft_usleep(1000, "[endcase_die_alone] usleep failed\n");
		ft_mutex_lock(&(state)->mtx_sim_state);
	}
	ft_mutex_unlock(&(state)->mtx_sim_state);
	put_left_fork(state, i);
	free(philosopher->arg);
	if (DEBUG == 1)
	{
		printf("🗿 😔 [endcase_die_alone]\n");
		printf("	👴 philo %d feels they had no chance to survive.\n", philosopher->id);
		printf("	💀 👼 philo_arg %d freed!\n", philosopher->id);
	}
	return ;
}

void	endcase_agony(t_state *state, t_philosopher *philosopher, int i)
{
	i++;i--; // cleanup
	if (state->settings.number_of_philosophers > 1)
	{
		ft_mutex_lock(&(state)->mtx_philo_all_fed_up);
		free(philosopher->arg);
		if (DEBUG == 1)
		{
			printf("\n🪦 😵 [endcase_die_agony]\n");
			printf("	👴 philo %d is lying on the floor...\n", philosopher->id);
			printf("	🤤 👼 philo_arg %d freed!\n", philosopher->id);
			printf("	... Shouldn't we take care of the body, though?\n");
		}
		philosopher->arg = NULL;
		ft_mutex_unlock(&(state)->mtx_philo_all_fed_up);
	}
	return ;
}

void	endcase_grief(t_state *state, t_philosopher *philosopher, int i)
{
	drop_forks(state, philosopher, i);
	ft_mutex_lock(&(state)->mtx_philo_all_fed_up);
	if (philosopher->arg == NULL)
	{
		ft_mutex_unlock(&(state)->mtx_philo_all_fed_up);
		return ;
	}
	else if (philosopher->arg != NULL)
	{
		free(philosopher->arg);	
		if (DEBUG == 1)
		{
			printf("\n❤️🩹 [endcase_grief]\n");
			printf("	👴 philo %d won't eat pasta out of ethical consideration.\n", philosopher->id);
			printf("	🕯️ 👋 philo_arg %d freed!\n", philosopher->id);
		}
		ft_mutex_unlock(&(state)->mtx_philo_all_fed_up);
	}
	return ;
}

void	endcase_satiety(t_state *state, t_philosopher *philosopher, int i)
{
	drop_forks(state, philosopher, i);
	free(philosopher->arg);
	if (DEBUG == 1)
	{
		printf("\n🍵🍃 [endcase_satiety]\n");
		printf("	👴 philo %d had %d meals, enough!\nThere is measure in all things.\n", philosopher->id, philosopher->meals_eaten);
		printf("	There is measure in all things.\n");	
		printf("	philo_arg %d freed!\n", philosopher->id);
	}
	return ;
}
