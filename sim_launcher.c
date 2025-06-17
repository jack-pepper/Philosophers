/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_launcher.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:11 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/17 09:55:54 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Set start time and start the clock
int	launch_death_clock(t_state *state)
{
	if (DEBUG == 1)
		printf("	\n 🕰️ [launch_death_clock] Launching death clock...\n");
	while (gettimeofday(&state->clock.cur_time, NULL) != 0)
		;
	set_start_time(state);
	if (pthread_create(&state->clock.thread, NULL, &clock_routine, state) != 0)
		return (1);
	if (DEBUG == 1)
		printf("	👍 [launch_death_clock] Death clock launched!\n");
	ft_usleep(1000, "[launch_death_clock] usleep failed\n");
	return (0);
}

int	create_philo_threads(t_state *state, int nb_guests)
{
	t_philo_arg	*arg;
	int			i;

	if (DEBUG == 1)
		printf("\n🧵 [create_philo_threads]\n");
	i = 0;
	while (i < nb_guests)
	{
		state->philosophers[i].arg = set_philo_arg(state, arg, i, nb_guests);
		arg = state->philosophers[i].arg;
		if (arg == NULL)
			return (1);
		if (pthread_create(&state->philosophers[i].thread, NULL,
				&philo_routine, arg) != 0)
		{
			free_philo_args(state, i); // testing
			return (1); 
		}
		if (DEBUG == 1)
			printf("	✅ thread philo %d created!\n", arg->i + 1);
		i++;
	}
	return (0);
}

t_philo_arg	*set_philo_arg(t_state *state, t_philo_arg *arg,
			int i, int nb_guests)
{
	arg = malloc(sizeof(t_philo_arg));
	if (!arg)
		return (NULL);
	arg->state = state;
	arg->i = i;
	arg->nb_guests = nb_guests;
	if (DEBUG == 1)
		printf("	✅ philo_arg %d set\n", arg->i + 1);
	return (arg);
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
			ft_putstr_fd(ERR_PHILO_THREAD_JOIN, STDERR);
		if (DEBUG == 1)
			printf("\n	✅ thread philo %d terminated!\n", i);
		i++;
	}
	return (0);
}
