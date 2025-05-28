/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:26:24 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/28 22:39:40 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    set_sim_status(t_state *state, bool sim_status)
{
        pthread_mutex_lock(&(state->mtx_sim_state));
        state->philo_all_set = sim_status;
        state->simulation_on = sim_status;
        pthread_mutex_unlock(&(state->mtx_sim_state));
}

int     is_sim_on(t_state *state)
{
        pthread_mutex_lock(&(state)->mtx_sim_state);
        if (state->simulation_on == false)
        {
                pthread_mutex_unlock(&(state)->mtx_sim_state);
                return (-1);
        }
        pthread_mutex_unlock(&(state)->mtx_sim_state);
        return (0);
}

int     wait_sim_start(t_state *state)
{
        pthread_mutex_lock(&(state->mtx_sim_state));
        while (!state->simulation_on)
        {
                pthread_mutex_unlock(&(state->mtx_sim_state));
                if (usleep(1000) != 0)
                        printf("[clock_routine] usleep failed\n");
                pthread_mutex_lock(&(state->mtx_sim_state));
                if (DEBUG == 1)
                        printf("        🔒 mtx_sim_state: locked (simulation_on is FALSE: clock is waiting...)!\n");
        }
        pthread_mutex_unlock(&(state->mtx_sim_state));
        if (DEBUG == 1)
                printf("        🔓 mtx_sim_state: unlocked (simulation_on is TRUE: clock goes on)!\n");
        return (0);
}
