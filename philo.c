/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:11 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/30 22:46:21 by mmalie           ###   ########.fr       */
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
		return (ft_ret(1, "[main] err: on initer()\n", STDERR));
	if (DEBUG == 1)
		printf("\n🎉 [main] State initiated!\n");
	if (launch_simulation(&state, state.settings.number_of_philosophers) != 0)
		return (ft_ret(1, "[main] err: on launch_simulation()\n", STDERR));
	if (DEBUG == 1)
		printf("\n╰┈➤🚪 [main] Exiting program.\n"); 
	return (free_on_exit(&state));
}

int	initer(t_state *state, int nb_guests)
{
	if (!state)
		return (ft_ret(1, "❌ [initer] err: !state\n", STDERR));
	if (init_barrier(state) != 0)
		return (ft_ret(1, "❌ [initer] err: init_barrier\n", STDERR));
	if (init_forks(state, nb_guests) != 0)
		return (ft_ret(1, "❌ [initer] err: init_forks\n", STDERR));
	if (DEBUG == 1)
		printf("🚀🍴 [initer] INIT_FORKS COMPLETE!\n");
	if (init_philosophers(state, nb_guests) != 0)
	{
		free(state->forks);
		return (ft_ret(1, "❌ [initer] err: init_philosophers\n", STDERR));
	}
	if (DEBUG == 1)
		printf("🚀👴 [initer] INIT_PHILOSOPHERS COMPLETE!\n");
	if (init_mutexes(state, nb_guests) != 0)
	{
		free(state->forks);
		free(state->philosophers);
		return (ft_ret(1, "❌ [initer] err: init_mutexes\n", STDERR));
	}
	if (DEBUG == 1)
		printf("🚀🔐 [initer] INIT_MUTEXES COMPLETE!\n");
	return (0);
}

int	launch_simulation(t_state *state, int nb_guests)
{
	if (DEBUG == 1)
		printf("\n🥽 [launch_simulation] Launching simulation...\n");
	(*state).philo_all_set = false;
	if (launch_death_clock(state) != 0)
		return (ft_ret(1, "❌ [launch_simulation] error in launch_death_clock!\n", STDERR));
	if (DEBUG == 1)
		printf("	✅ [launch_simulation] death clock launched!\n");

	if (create_philo_threads(state, nb_guests) != 0)
		return (ft_ret(1, "❌ [launch_simulation] error in create_philo_threads!\n", STDERR));
	if (DEBUG == 1)
		printf("	✅ [launch_simulation] philo threads created!\n");	
	if (pthread_join(state->clock.thread, NULL) != 0)
		return (1);
	if (DEBUG == 1)
		printf("        ✅ [launch_simulation] thread clock terminated!\n");
	if (join_philo_threads(state, nb_guests) != 0)
		return (ft_ret(1, "❌ [launch_simulation] error in join_philo_threads!\n", STDERR));
	if (DEBUG == 1)
		printf("	✅ [launch_simulation] philo threads all terminated!\n");
	return (0);
}
