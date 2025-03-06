#include "philo.h"

int		main(int argc, char **argv)
{
	t_state			*state;
	int				res;
	
	state = malloc(sizeof(t_state));
	if (!state)
		return (-1);
	memset(state, 0, sizeof(t_state));
	//if (args_are_valid(argc, argv, &state.settings) != 0) // ADD PARSING AFTER
	//	return (-1);
	//printf("Args are valid!\n");
	printf("argc: %d\n", argc); // temporary for DEBUG
	store_args(argv, &(*state).settings);
	printf("Args stored! %d %d %d %d %d\n", (*state).settings.number_of_philosophers, (*state).settings.time_to_die,
		(*state).settings.time_to_eat, (*state).settings.time_to_sleep, (*state).settings.number_of_times_each_philosopher_must_eat);
	res = 0;
	res = start_council(&state, &(*state).philosophers, &(*state).forks);
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

int	start_council(t_state **state, t_philosopher **philosophers, t_fork **forks)
{
	int	nb_guests;
	int i;

	nb_guests = (*state)->settings.number_of_philosophers;
	if (pthread_mutex_init(&(*state)->mutex_start_simulation, NULL) != 0)
        return (-1);
	pthread_mutex_lock(&(*state)->mutex_start_simulation);
	(*state)->simulation_on = false;
	if (set_clock(state) != 0)
	{
		printf("[start_council] Clock non set!\n"); // DEBUG
		return (-1);
	}
	printf("[start_council] Clock set!\n"); // DEBUG
	if (set_forks(&*forks, nb_guests) != 0)
	{
		printf("[start_council] Forks non set!\n");// DEBUG
		return (-1);
	}
	printf("[start_council] Forks set!\n"); // DEBUG
	if (set_philosophers(state, &*philosophers, nb_guests) != 0)
	{
		printf("[start_council] Philosophers non set!\n"); // DEBUG
		return (-1);
	}
	(*state)->simulation_on = true;
	pthread_mutex_unlock(&(*state)->mutex_start_simulation);
	// destroy_mutexes()
	i = 0;
	while (i < nb_guests)
	{
		if (pthread_mutex_destroy(&(*forks)[i].mutex) != 0)
		return (-1); // Better to wait?
	}
	return (0);
}