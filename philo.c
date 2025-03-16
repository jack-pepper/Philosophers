/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:11 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/17 00:03:28 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_state	state;

	memset(&state, 0, sizeof(t_state));
	// PARSING ARGS LOGIC HERE
	printf("argc: %d\n", argc); // temp for DEBUG
	store_args(argv, &state.settings);
	display_settings(&state.settings);	
	if (initer(&state, state.settings.number_of_philosophers) != 0)
		return (ft_ret(-1, "[main] err: on initer\n"));
	if (launch_simulation(&state, state.settings.number_of_philosophers) != 0)
		return (ft_ret(-1, "[main] err: on launch_simulation\n"));
	printf("[main] exit program (0)\n");
        return (free_on_exit(&state));
}

int	initer(t_state *state, int nb_guests)
{
	if (!state)
		return (ft_ret(-1, "[initer] err: !state\n"));
	if (init_forks(state, nb_guests) != 0)
		return (ft_ret(-1, "[initer] err: init_forks\n"));
	if (init_philosophers(state, nb_guests) != 0)
	{
		free(state->forks);
		return (ft_ret(-1, "[initer] err: init_philosophers\n"));
	}
	if (init_mutexes(state, nb_guests) != 0)
	{
		free(state->forks);
		free(state->philosophers);
		return (ft_ret(-1, "[initer] err: init_mutexes\n"));
	}
	return (0);
}

int	launch_simulation(t_state *state, int nb_guests)
{
	t_philo_arg	*arg;
	int	i;

	(*state).philo_all_set = false;
	printf("[launch_simulation] (after setting to false) philo_all_set: %d\n", (*state).philo_all_set);
	if (launch_death_clock(state) != 0)
		return (-1);
	i = 0;
	while (i < nb_guests)
	{
		arg = malloc(sizeof(t_philo_arg));
		if (!arg)
			return (-1);
		arg->state = state;
		arg->i = i;
		arg->nb_guests = nb_guests;
		if (pthread_create(&state->philosophers[i].thread, NULL, &philo_routine, arg) != 0)
		{
			// free memory depending of i
			return (1);
		}
		
		i++;
	}
	//usleep(10000);
	(*state).philo_all_set = true;
	printf("[launch_simulation (after setting to true)] philo_all_set: %d\n", (*state).philo_all_set);
	printf("[launch_simulation] created philosophers' threads\n");	
	(*state).simulation_on = true;
	i = 0;
	while (i < nb_guests)
	{
		if (pthread_join(state->philosophers[i].thread, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
                state->philosophers[i].last_meal_time_ms = get_timestamp_ms(&state->clock.cur_time);
		printf("last_meal: %ld\n", state->philosophers[i].last_meal_time_ms);
		i++;
	}
	printf("[launch_simulation] joined philosophers pthreads!\n");
	// calculate offset (difference between clock starting time and philosophers' launching to start at 0)
//	if (pthread_join(state->clock.thread, NULL) != 0)
//	{
		// free memory depending of i
//		return (1);
//	}
	return (0);
}

// Set start time and start the clock
int	launch_death_clock(t_state *state)
{
	if (gettimeofday(&state->clock.cur_time, NULL) != 0)
		printf("[launch_death_clock] gettimeofday fail\n"); // check in a loop while struct == 0?
	state->clock.cur_time_ms = convert_to_ms(state->clock.cur_time);
	state->clock.start_time_ms = state->clock.cur_time_ms;
    	printf("[launch_death_clock] cur_time_ms: %lu - start_time_ms: %lu\n", state->clock.cur_time_ms, state->clock.start_time_ms);
	if (pthread_create(&state->clock.thread, NULL, &clock_routine, state) != 0)
		return (-1);
	return (0);
}
