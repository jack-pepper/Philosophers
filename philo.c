/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:11 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/27 12:08:25 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_state	state;

	memset(&state, 0, sizeof(t_state));
	// PARSING ARGS LOGIC HERE
	
	if (DEBUG == 1)
		printf("argc: %d\n", argc); // temp for DEBUG
	store_args(argv, &state.settings);
	if (DEBUG == 1)
		display_settings(&state.settings);	
	if (initer(&state, state.settings.number_of_philosophers) != 0)
		return (ft_ret(-1, "[main] err: on initer\n"));
	if (DEBUG == 1)
		printf("[MAIN] INITER COMPLETE! ... LAUNCHING SIMULATION...\n");
	if (launch_simulation(&state, state.settings.number_of_philosophers) != 0)
		return (ft_ret(-1, "[main] err: on launch_simulation\n"));
	if (DEBUG == 1)
		printf("[MAIN] SIMULATION ENDING... Exiting program.\n");
        return (free_on_exit(&state));
}

int	initer(t_state *state, int nb_guests)
{
	if (!state)
		return (ft_ret(-1, "[initer] err: !state\n"));
	if (init_barrier(state) != 0)
		return (ft_ret(-1, "[initer] err: init_barrier\n"));
	if (init_forks(state, nb_guests) != 0)
		return (ft_ret(-1, "[initer] err: init_forks\n"));
	if (DEBUG == 1)
		printf("[initer] INIT_FORKS COMPLETE!\n");
	if (init_philosophers(state, nb_guests) != 0)
	{
		free(state->forks);
		return (ft_ret(-1, "[initer] err: init_philosophers\n"));
	}
	if (DEBUG == 1)
		printf("[initer] INIT_PHILOSOPHERS COMPLETE!\n");
	if (init_mutexes(state, nb_guests) != 0)
	{
		free(state->forks);
		free(state->philosophers);
		return (ft_ret(-1, "[initer] err: init_mutexes\n"));
	}
	if (DEBUG == 1)
		printf("[initer] INIT_MUTEXES COMPLETE!\n");
	return (0);
}

int	launch_simulation(t_state *state, int nb_guests)
{
	t_philo_arg	*arg;
	int	i;

	(*state).philo_all_set = false;
	
	// create_philo_threads() 
	i = 0;
	while (i < nb_guests)
	{
		arg = malloc(sizeof(t_philo_arg));
		if (!arg)
			return (-1);
		arg->state = state;
		arg->i = i;
		arg->nb_guests = nb_guests;
		if (DEBUG == 1)
			printf("[launch_simulation] philo %d initialized\n", arg->i);
		if (pthread_create(&state->philosophers[i].thread, NULL, &philo_routine, arg) != 0)
		{
			// free memory depending of i
			return (1);
		}
		if (DEBUG == 1)
			printf("[launch_simulation] thread philo %d created!\n", arg->i);
		i++;
	}

	//
	if (launch_death_clock(state) != 0) // shouldn't be later? or calc an offset...
		return (-1);
	if (DEBUG == 1)
		printf("[launch_simulation] death clock launched!\n");

	// join_philo_threads()
	i = 0;
	while (i < nb_guests)
	{
		if (pthread_join(state->philosophers[i].thread, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
		if (DEBUG == 1)
			printf("[launch_simulation] thread philo %d joined!\n", i);
                state->philosophers[i].last_meal_time_ms = get_timestamp_ms(&state->clock.cur_time);
		if (DEBUG == 1)
			printf("last_meal: %ld\n", state->philosophers[i].last_meal_time_ms);
		i++;
	}

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
	if (DEBUG == 1)
		printf("[launch_death_clock] LAUNCHING DEATH CLOCK!\n");
	if (gettimeofday(&state->clock.cur_time, NULL) != 0 && DEBUG == 1)
		printf("[launch_death_clock] gettimeofday fail\n"); // check in a loop while struct == 0?
	state->clock.cur_time_ms = convert_to_ms(state->clock.cur_time);
	state->clock.start_time_ms = state->clock.cur_time_ms;
    	if (DEBUG == 1)
		printf("[launch_death_clock] cur_time_ms: %lu - start_time_ms: %lu\n",
			state->clock.cur_time_ms, state->clock.start_time_ms);
	if (pthread_create(&state->clock.thread, NULL, &clock_routine, state) != 0)
		return (-1);
	if (DEBUG == 1)
		printf("[🕰️] [launch_death_clock] DEATH CLOCK LAUNCHED SUCCESSFULLY!\n");
	return (0);
}
