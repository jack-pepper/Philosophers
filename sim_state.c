/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:26:24 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/16 07:41:43 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_sim_status(t_state *state, bool sim_status)
{
	ft_mutex_lock(&(state)->mtx_sim_state);
	state->simulation_on = sim_status;
	ft_mutex_unlock(&(state)->mtx_sim_state);
}

int	is_sim_on(t_state *state)
{
	ft_mutex_lock(&(state)->mtx_sim_state);
	if (state->simulation_on == false)
	{
		ft_mutex_unlock(&(state)->mtx_sim_state);
		return (1);
	}
	ft_mutex_unlock(&(state)->mtx_sim_state);
	return (0);
}

void	set_philo_threads_all_set(t_state *state, bool set_status)
{
	ft_mutex_lock(&(state)->mtx_philo_all_set);
	state->philo_all_set = set_status;
	ft_mutex_unlock(&(state)->mtx_philo_all_set);
}

int	are_philo_threads_all_set(t_state *state)
{
	ft_mutex_lock(&(state)->mtx_philo_all_set);
	if (state->philo_all_set == false)
	{
		ft_mutex_unlock(&(state)->mtx_philo_all_set);
		return (1);
	}
	ft_mutex_unlock(&(state)->mtx_philo_all_set);
	return (0);
}

int	are_philo_all_fed_up(t_state *state)
{
	if (state->settings.number_of_times_each_philosopher_must_eat < 1)
		return (0);
	if (state->settings.number_of_times_each_philosopher_must_eat > 0)
	{
		ft_mutex_lock(&(state)->mtx_philo_all_fed_up);
		if (state->philo_all_fed_up == false)
		{
			ft_mutex_unlock(&(state)->mtx_philo_all_fed_up);
			return (0);
		}
		ft_mutex_unlock(&(state)->mtx_philo_all_fed_up);
	}
	return (1);
}
