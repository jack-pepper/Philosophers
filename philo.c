/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:11 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/27 21:35:10 by mmalie           ###   ########.fr       */
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
		printf("\n🎉 [MAIN] INITER COMPLETE! ... Launching simulation...\n");
	if (launch_simulation(&state, state.settings.number_of_philosophers) != 0)
		return (ft_ret(-1, "[main] err: on launch_simulation\n"));
	if (DEBUG == 1)
		printf("\n╰┈➤🚪 [MAIN] SIMULATION ENDING... Exiting program.\n");
        return (free_on_exit(&state));
}

int	initer(t_state *state, int nb_guests)
{
	if (!state)
		return (ft_ret(-1, "❌ [initer] err: !state\n"));
	if (init_barrier(state) != 0)
		return (ft_ret(-1, "❌ [initer] err: init_barrier\n"));
	if (init_forks(state, nb_guests) != 0)
		return (ft_ret(-1, "❌ [initer] err: init_forks\n"));
	if (DEBUG == 1)
		printf("🚀🍴 [initer] INIT_FORKS COMPLETE!\n");
	if (init_philosophers(state, nb_guests) != 0)
	{
		free(state->forks);
		return (ft_ret(-1, "❌ [initer] err: init_philosophers\n"));
	}
	if (DEBUG == 1)
		printf("🚀👴 [initer] INIT_PHILOSOPHERS COMPLETE!\n");
	if (init_mutexes(state, nb_guests) != 0)
	{
		free(state->forks);
		free(state->philosophers);
		return (ft_ret(-1, "❌ [initer] err: init_mutexes\n"));
	}
	if (DEBUG == 1)
		printf("🚀🔐 [initer] INIT_MUTEXES COMPLETE!\n");
	return (0);
}

int	create_philo_threads(t_state *state, int nb_guests)
{
	t_philo_arg	*arg;
	int		i;

	if (DEBUG == 1)
		printf("\n🧵 [create_philo_threads]\n");
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
			printf("	✅ philo %d initialized\n", arg->i);
		if (pthread_create(&state->philosophers[i].thread, NULL, &philo_routine, arg) != 0)
		{
			// free memory depending of i
			return (1);
		}
		if (DEBUG == 1)
			printf("	✅ thread philo %d created!\n", arg->i);
		i++;
	}
	return (0);
}

int	join_philo_threads(t_state *state, int nb_guests)
{
	int	i;

	i = 0;
	if (DEBUG == 1)
		printf("\n 🪡 [join_philo_threads]\n");
	while (i < nb_guests)
	{
		if (pthread_join(state->philosophers[i].thread, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
		if (DEBUG == 1)
			printf("	✅ thread philo %d joined!\n", i);
                state->philosophers[i].last_meal_time_ms = get_timestamp_ms(&state->clock.cur_time);
		if (DEBUG == 1)
			printf("	😫 last_meal: %ld\n", state->philosophers[i].last_meal_time_ms);
		i++;
	}
	return (0);
}


int	launch_simulation(t_state *state, int nb_guests)
{
	(*state).philo_all_set = false;
	if (create_philo_threads(state, nb_guests) != 0)
	{
		printf("❌ [launch_simulation] error in create_philo_threads!\n");
		return (-1);
	}
	if (DEBUG == 1)
		printf("✅ [launch_simulation] philo threads created!\n");
	if (launch_death_clock(state) != 0) // shouldn't be later? or calc an offset...
	{
		printf("❌ [launch_simulation] error in launch_death_clock!\n");
		return (-1);
	}
	if (DEBUG == 1)
		printf("✅ [launch_simulation] death clock launched!\n");
	if (join_philo_threads(state, nb_guests) != 0)
	{
		printf("❌ [launch_simulation] error in join_philo_threads!\n");
		return (-1);
	}
	if (DEBUG == 1)
		printf("✅ [launch_simulation] philo threads joined!\n");
	return (0);
}

// Set start time and start the clock
int	launch_death_clock(t_state *state)
{
	if (DEBUG == 1)
		printf("🕰️ [launch_death_clock] LAUNCHING DEATH CLOCK!\n");
	if (gettimeofday(&state->clock.cur_time, NULL) != 0 && DEBUG == 1)
		printf("	❌ gettimeofday fail\n"); // check in a loop while struct == 0?
	pthread_mutex_lock(&(state->clock.mtx_get_time));
	state->clock.cur_time_ms = convert_to_ms(state->clock.cur_time);
	state->clock.start_time_ms = state->clock.cur_time_ms;
    	pthread_mutex_unlock(&(state->clock.mtx_get_time));
	if (DEBUG == 1)
		printf("	⌚ cur_time_ms: %lu - start_time_ms: %lu\n",
			state->clock.cur_time_ms, state->clock.start_time_ms);
	if (pthread_create(&state->clock.thread, NULL, &clock_routine, state) != 0)
		return (-1);
	if (DEBUG == 1)
		printf("	👍 [launch_death_clock] DEATH CLOCK LAUNCHED SUCCESSFULLY!\n");
	return (0);
}
