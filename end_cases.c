/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_cases.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 10:03:31 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/21 22:58:27 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		ft_usleep(state, 1000, "[endcase_die_alone] usleep failed\n");
		ft_mutex_lock(&(state)->mtx_sim_state);
	}
	ft_mutex_unlock(&(state)->mtx_sim_state);
	put_left_fork(state, i);
	free(philosopher->arg);
	if (DEBUG == 1)
		endcase_msg(philosopher, "die_alone");
	return ;
}

void	endcase_agony(t_state *state, t_philosopher *philosopher)
{
	if (state->settings.number_of_philosophers > 1)
	{
		ft_mutex_lock(&(state)->mtx_philo_all_fed_up);
		free(philosopher->arg);
		if (DEBUG == 1)
			endcase_msg(philosopher, "agony");
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
			endcase_msg(philosopher, "grief");
		ft_mutex_unlock(&(state)->mtx_philo_all_fed_up);
	}
	return ;
}

void	endcase_satiety(t_state *state, t_philosopher *philosopher, int i)
{
	drop_forks(state, philosopher, i);
	free(philosopher->arg);
	if (DEBUG == 1)
		endcase_msg(philosopher, "satiety");
	return ;
}

void	endcase_msg(t_philosopher *philo, char *endcase)
{
	if (ft_strcmp(endcase, "die_alone") == 0)
	{
		printf("🗿 😔 [endcase_die_alone]\n");
		printf("	👴 philo %d had no escape...\n", philo->id);
		printf("	💀 👼 philo_arg %d freed!\n", philo->id);
	}
	else if (ft_strcmp(endcase, "agony") == 0)
	{
		printf("\n🪦 😵 [endcase_die_agony]\n");
		printf("        👴 philo %d didn't make it.\n", philo->id);
		printf("        🤤 👼 philo_arg %d freed!\n", philo->id);
	}
	else if (ft_strcmp(endcase, "grief") == 0)
	{
		printf("\n❤️🩹 [endcase_grief]: Ethics first!\n");
		printf("        👴 philo %d won't eat.\n", philo->id);
		printf("        🕯️  👋 philo_arg %d freed!\n", philo->id);
	}
	else if (ft_strcmp(endcase, "satiety") == 0)
	{
		printf("\n🍵🍃 [endcase_satiety]: Measure, please!\n");
		printf("        👴 philo %d had %d meals. It's enough!\n",
			philo->id, philo->meals_eaten);
		printf("        philo_arg %d freed!\n", philo->id);
	}
}
