#include "philo.h"

int		main(int argc, char **argv)
{
	t_state			*state;
	int				res;

	// malloc and init state to 0
	state = malloc(sizeof(t_state));
	if (!state)
		return (-1);
	memset(state, 0, sizeof(t_state));

	// check args validity
	//if (args_are_valid(argc, argv, &state.settings) != 0) // ADD PARSING AFTER
	//	return (-1);
	//printf("Args are valid!\n");
	printf("argc: %d\n", argc); // temporary for DEBUG

	// store valid args to settings
	store_args(argv, &(*state).settings);
	printf("Args stored! %d %d %d %d %d\n", (*state).settings.number_of_philosophers, (*state).settings.time_to_die,
		(*state).settings.time_to_eat, (*state).settings.time_to_sleep, (*state).settings.number_of_times_each_philosopher_must_eat);
	
	// start council (set clock, forks and philosophers)
	res = 0;
	res = start_council(&state);

	// handle the res of start_council
	if (res == -1)
	{
		printf("[main.c] Error: xxx\n");
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

	// Clean up and Destroy mutexes
	res = 0;
	while (res < (*state).settings.number_of_philosophers)
	{
		if (pthread_mutex_destroy(&(*state).forks[res].mutex) != 0)
			return (-1); // Better to wait?
	}
}

int	start_council(t_state **state)
{
	int	nb_guests;
	//int i;

	printf("[start_council] Entering start_council...\n");
	nb_guests = (*state)->settings.number_of_philosophers;
	printf("[start_council] Get nb_guests (%d) from settings (%d)\n", nb_guests, (*state)->settings.number_of_philosophers);
    if (pthread_mutex_init(&(*state)->mutex_start_simulation, NULL) != 0)
        	return (-1);
	pthread_mutex_lock(&(*state)->mutex_start_simulation);
	printf("MUTEX_START_SIMULATION locked by: main thread\n");
	(*state)->simulation_on = false;
	printf("[start_council] Setting clock...\n");
	if (set_clock(state) != 0)
	{
		printf("[start_council] Clock non set!\n"); // DEBUG
			return (-1);
	}
	printf("[start_council] Clock set!\n"); // DEBUG
	(*state)->simulation_on = true;
	printf("[start_council] Setting forks...\n");
	if (set_forks(state, nb_guests) != 0)
	{
		printf("[start_council] Forks non set!\n");// DEBUG
			return (-1);
	}
	printf("[start_council] Forks set!\n"); // DEBUG
	printf("[start_council] Setting philosophers...\n");
	if (set_philosophers(state, nb_guests) != 0)
	{
		printf("[start_council] Philosophers non set!\n"); // DEBUG
			return (-1);
	}
	printf("[start_council] Philosophers set!\n"); // DEBUG	
	pthread_mutex_unlock(&(*state)->mutex_start_simulation);
	printf("MUTEX_START_SIMULATION UNlocked by: main thread\n");

	return (0);
}
