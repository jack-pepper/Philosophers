#include "philo.h"

int		main(int argc, char **argv)
{
	t_state			state = {0};
	int				res;
	
	// memset(&settings, 0, sizeof(settings)); // If {0} init not allowed by norm
	//if (args_are_valid(argc, argv, &state.settings) != 0) // ADD PARSING AFTER
	//	return (-1);
	//printf("Args are valid!\n");
	printf("argc: %d\n", argc); // temporary for DEBUG
	store_args(argv, &state.settings);
	printf("Args have been stored!\n");
	res = 0;
	res = start_council(&state, state.philosophers, state.forks);
	if (res == -1)
	{
		printf("Error: xxx\n");
		return (-1); // Error: process not launched, problem with locks...
	}
	else if (res == 1)
	{
		printf("Error: xxx\n");
		return (1); // Philosopher died
	}
		else if (res == 2)
	{
		printf("Error: xxx\n");
		return (2); // All required meals eaten
	}
}

int	start_council(t_state *state, t_philosopher *philosophers, t_fork *forks)
{
	int	nb_guests;
	int i;
	
	nb_guests = state->settings.number_of_philosophers;
	if (set_clock(state) != 0)
	{
		printf("[start_council] Clock non set!\n");// DEBUG
		return (-1);
	}
	if (set_forks(state->forks, nb_guests) != 0)
	{
		printf("[start_council] Forks non set!\n");// DEBUG
		return (-1);
	}
	if (set_philosophers(state, philosophers, nb_guests) != 0)
	{
		printf("[start_council] Philosophers non set!\n");// DEBUG
		return (-1);
	}
	// destroy_mutexes()
	i = 0;
	while (i < nb_guests)
	{
		if (pthread_mutex_destroy(&forks[i].mutex) != 0)
		return (-1); // Better to wait?
	}
	return (0);
}