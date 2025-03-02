#include "philo.h"


int	main(int argc, char **argv)
{
	t_philo_vars	settings = {0};
	int				res;
	
	// If {0} init not allowed, I will use memset
	// memset(&settings, 0, sizeof(settings));
	
	if (args_are_valid(argc, argv, &settings) != 0)
	return (-1);
	store_args(argv, &settings);
	
	res = 0;
	res = start_council(&settings); // Won't stop
	
	if (res == -1)
	return (-1); // Error: process not launched, problem with locks...
	else if (res == 1)
	return (1); // Philosopher died
	else if (res == 2)
	return (2); // All required meals eaten
	
	
}

int	start_council(t_philosopher *philosophers, t_philo_vars *settings)
{
	t_fork	*forks;
	t_philosopher	*philosophers;
	
	uint64_t timestamp_ms;

	int i;

	// set_forks() and mutexes;
	forks =  malloc(sizeof(t_fork) * settings->number_of_philosophers);
	if (!forks)
		return (-1);
	i = 0;
	while (i < settings->number_of_philosophers)
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
	
	// set philosophers()
	philosophers = malloc(sizeof(t_philosopher) * settings->number_of_philosophers);
	if (!philosophers)
		return (-1);
	i = 0;
	while (i < settings->number_of_philosophers)
	{
		if (pthread_create(philosophers[i].thread, NULL, &routine, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
		i++;
		philosophers[i].id = i + 1;
	}
	
	i = 0;
	while (i < settings->number_of_philosophers)
	{
		if (pthread_join(philosophers[i].thread, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
		i++;
	}
	
	
	pthread_mutex_destroy(&mutex);
}

void	*routine() {
	pthread_mutex_lock(&mutex);
	
	pthread_mutex_unlock(&mutex);
}

