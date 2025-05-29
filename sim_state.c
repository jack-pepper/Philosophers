/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:26:24 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/29 20:08:33 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    set_sim_status(t_state *state, bool sim_status)
{
        pthread_mutex_lock(&(state)->mtx_sim_state);
        state->simulation_on = sim_status;
        pthread_mutex_unlock(&(state)->mtx_sim_state);
}

void	set_philo_threads_all_set(t_state *state, bool set_status)
{
        pthread_mutex_lock(&(state)->mtx_philo_all_set);
        state->philo_all_set = set_status;
        pthread_mutex_unlock(&(state)->mtx_philo_all_set);
}

int     is_sim_on(t_state *state)
{
        pthread_mutex_lock(&(state)->mtx_sim_state);
        if (state->simulation_on == false)
        {
                pthread_mutex_unlock(&(state)->mtx_sim_state);
                return (1);
        }
        pthread_mutex_unlock(&(state)->mtx_sim_state);
        return (0);
}

int	are_philo_threads_all_set(t_state *state)
{
        pthread_mutex_lock(&(state)->mtx_philo_all_set);
        if (state->philo_all_set == false)
        {
                pthread_mutex_unlock(&(state)->mtx_philo_all_set);
                return (1);
        }
        pthread_mutex_unlock(&(state)->mtx_philo_all_set);
        return (0);
}

int     wait_sim_start(t_state *state)
{
        pthread_mutex_lock(&(state->mtx_sim_state));
        while (!state->simulation_on)
        {
                pthread_mutex_unlock(&(state->mtx_sim_state));
                if (usleep(1000) != 0)
                        printf("[wait sim_start] usleep failed\n");
                pthread_mutex_lock(&(state->mtx_sim_state));
                if (DEBUG == 1)
                        printf("        🔒 mtx_sim_state: locked (simulation_on is FALSE: waiting...)\n");
	}
        pthread_mutex_unlock(&(state->mtx_sim_state));
        if (DEBUG == 1)
                printf("        🔓 mtx_sim_state: unlocked (simulation_on is TRUE: let's go!)\n");
        return (0);
}
