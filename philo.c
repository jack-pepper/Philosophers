/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:11 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/22 22:57:39 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_state	state;

	memset(&state, 0, sizeof(t_state));
	if (are_args_valid(argc, argv) != 0)
		return (free_on_exit(&state));
	store_args(argv, &state.settings);
	set_optimal_delay(&state.settings);
	if (DEBUG == 1)
		display_settings(&state.settings);
	if (initer(&state, state.settings.number_of_philosophers) != 0)
		return (ft_ret(1, ERR_INITER, STDERR));
	if (DEBUG == 1)
		printf(SUC_INITER);
	if (launch_simulation(&state, state.settings.number_of_philosophers) != 0)
		ft_putstr_fd(ERR_LAUNCH_SIMULATION, STDERR);
	if (DEBUG == 1)
		printf(EXIT_MESSAGE);
	return (free_on_exit(&state));
}

int	initer(t_state *state, int nb_guests)
{
	if (!state)
		return (ft_ret(1, ERR_STATE, STDERR));
	if (init_barrier(state) != 0)
		return (ft_ret(1, ERR_INIT_BARRIER, STDERR));
	if (init_forks(state, nb_guests) != 0)
		return (ft_ret(1, ERR_INIT_FORKS, STDERR));
	if (init_philosophers(state, nb_guests) != 0)
	{
		free(state->forks);
		return (ft_ret(1, ERR_INIT_PHILOSOPHERS, STDERR));
	}
	if (init_mutexes(state, nb_guests) != 0)
	{
		free(state->forks);
		free(state->philosophers);
		return (ft_ret(1, ERR_INIT_MUTEXES, STDERR));
	}
	return (0);
}

int	launch_simulation(t_state *state, int nb_guests)
{
	if (DEBUG == 1)
		printf(SUC_LAUNCH_SIMULATION);
	(*state).dead_spotted = false;
	(*state).philo_all_set = false;
	(*state).philo_all_fed_up = false;
	if (launch_death_clock(state) != 0)
		return (ft_ret(1, ERR_LAUNCH_DEATH_CLOCK, STDERR));
	if (DEBUG == 1)
		printf(SUC_LAUNCH_DEATH_CLOCK);
	if (create_philo_threads(state, nb_guests) == 0 && DEBUG == 1)
		printf(SUC_CREATE_PHILO_THREADS);
	if (pthread_join(state->clock.thread, NULL) != 0)
		return (ft_ret(1, ERR_JOIN_CLOCK_THREAD, STDERR));
	if (DEBUG == 1)
		printf(SUC_JOIN_CLOCK_THREAD);
	if (join_philo_threads(state, nb_guests) == 0 && DEBUG == 1)
		printf(SUC_JOIN_PHILO_THREADS);
	return (0);
}
