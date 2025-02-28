#include "philo.h"


int	main(int argc, char **argv)
{
	t_philo_vars	settings = {0};
	t_philosopher	*philosophers;
	int res;
	
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

// Try: init_philosopher / init_joins / init_mutexes...
int	start_council(t_philosopher *philosophers, t_philo_vars *settings)
{
	pthread_mutex_t	mutex;

	uint64_t timestamp_ms;
	int i;
		
	pthread_mutex_init(&mutex, NULL);
	

	i = 0;
	while (i < settings->number_of_philosophers)
	{
		if (pthread_create(philosophers[i]->thread, NULL, &routine, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
		i++;
		philosophers[i]->id = i + 1;
		
	}
	
	i = 0;
	while (i < settings.number_of_philosophers)
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

