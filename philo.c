/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:11 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/07 23:37:30 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_state	state;

	memset(&state, 0, sizeof(t_state));
	printf("argc: %d\n", argc); // temp for DEBUG
	store_args(argv, &state.settings);
	printf("Args stored! %d %d %d %d %d\n", state.settings.number_of_philosophers, state.settings.time_to_die,
	state.settings.time_to_eat, state.settings.time_to_sleep, state.settings.number_of_times_each_philosopher_must_eat);
	if (initer(&state) != 0)
	{
		printf("[main] error on initer\n");
		return (-1);
	}
	if (launch_simulation(&state) != 0)
	{
		printf("[main] pthread error on launch_simulation)");
		return (-1);
	}
	return (0);
}

int     initer(t_state *state)
{
        int     nb_guests;

        nb_guests = state->settings.number_of_philosophers;
        printf("[initer] nb_of_guests: %d\n", nb_guests);
        if (!state)
                return (-1);
       // if (init_clock(state) != 0) // Probably better to do at the hand or in the launcher?
       //         return (-1);
        if (init_forks(state, nb_guests) != 0)
                return (-1);
        if (init_philosophers(state, nb_guests) != 0)
                return (-1);
        if (init_mutexes(state, nb_guests) != 0)
                return (-1);
        return (0);
}

int	launch_simulation(t_state *state)
{
	t_philo_arg	*arg;	
	int     nb_guests;
	int	i;

        nb_guests = state->settings.number_of_philosophers;
        printf("[launch_simulation] nb_of_guests: %d\n", nb_guests);
	if (launch_death_clock(state, nb_guests) != 0)
		return (-1);
	i = 0;
	while (i < nb_guests)
	{
		arg = malloc(sizeof(arg));
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

	//if (pthread_join(state->clock.thread, NULL) != 0)
	//	return (-1);
	i = 0;
	while (i < nb_guests)
	{
		if (pthread_join(state->philosophers[i].thread, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
		i++;
	}
	return (0);
}

// Set start time and start the clock
int     launch_death_clock(t_state *state, int nb_guests)
{
	int	i;

	i = 0;
	if (gettimeofday(&state->clock.cur_time, NULL) != 0)
		printf("[launch_death_clock] gettimeofday fail\n"); // check in a loop while struct == 0?
    	state->clock.cur_time_ms = convert_to_ms(state->clock.cur_time);
    	state->clock.start_time_ms = state->clock.cur_time_ms;
    	printf("[launch_death_clock] cur_time_ms: %lu - start_time_ms: %lu\n", state->clock.cur_time_ms, state->clock.start_time_ms);
    	while (i < nb_guests)
	{
		state->philosophers[i].last_meal_time_ms = state->clock.cur_time_ms;
		printf("philosopher[%d] last_meal_time_ms: %lu\n", i, state->philosophers[i].last_meal_time_ms);
		i++;
	}
    	if (pthread_create(&state->clock.thread, NULL, &clock_routine, state) != 0)
        	return (-1);
    	return (0);
}
