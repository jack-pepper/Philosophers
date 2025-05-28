/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gandalf_barrier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:12:19 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/28 23:44:11 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	gandalf_barrier(t_state *state)
{
	if (DEBUG == 1)
		printf("\n🧙🚧 [gandalf_barrier]\n");
	pthread_mutex_lock(&(state->barrier.mtx_barrier));
	state->threads_ready += 1;
	if (DEBUG == 1)
		printf("    🧵 threads_ready: ++ (%d)\n", state->threads_ready);

	if (state->threads_ready < state->barrier.threshold)
	{
		pthread_mutex_unlock(&(state->barrier.mtx_barrier)); // can return err too?
		if (wait_sim_start(state) != 0)
			return ;
		return ;
	}
	else
        {
		set_sim_status(state, true);
                pthread_mutex_unlock(&(state->barrier.mtx_barrier)); // can return err too?
                pthread_mutex_destroy(&(state->barrier.mtx_barrier));
                if (DEBUG == 1)
                        printf("	\n💥 Gandalf barrier destroyed!\n");
        }
        return ;
}
