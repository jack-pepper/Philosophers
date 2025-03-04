#include "philo.h"

int	set_philosophers(t_state *state, t_philosopher *philosophers, int nb_guests)
{
	int	i;

	// malloc philosophers
	philosophers = malloc(sizeof(t_philosopher) * nb_guests);
	if (!philosophers)
		return (-1);
	i = 0;
	
	// create threads
	while (i < nb_guests)
	{
		state->current_i = i;
		if (pthread_create(philosophers[i].thread, NULL, &routine, &state) != 0)
		{
			// free memory depending of i
			return (1);
		}
		i++;
		philosophers[i].id = i + 1;
	}

	// join threads
	i = 0;
	while (i < nb_guests)
	{
		if (pthread_join(philosophers[i].thread, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
		i++;
	}
}

int	set_forks(t_fork *forks, int nb_guests)
{
	int	i;

	// malloc for forks
	forks =  malloc(sizeof(t_fork) * nb_guests);
	if (!forks)
		return (-1);

	// init fork mutexes
	i = 0;
	while (i < nb_guests)
	{
		forks[i].id = i + 1;
		forks[i].is_already_taken = false;
		if (pthread_mutex_init(&forks[i].mutex, NULL) != 0)
		{
			// free forks
			return (-1);
		}
	i++;
	}
}