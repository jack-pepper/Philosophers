
/*typedef struct s_philosopher
{
	int	id;
	int	meals_eaten;
	int	leaving;
	int	eating;
	int	sleeping;
	int	thinking;
}		t_philosopher;
*/

#include <pthread.h>
#include <stdint.h>

typedef struct s_philo_vars
{
	int number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
}		t_philo_vars;

void	*routine() {
	pthread_mutex_lock(&mutex);
	//
	pthread_mutex_unlock(&mutex);
}

int	main(int argc, char **argv)
{
	t_philo_vars	settings;
	pthread_t	*philosophers;
	pthread_mutex_t	mutex;
	
	uint64_t timestamp_ms;
	int i;
	
	if (args_are_valid(argc, argv, &settings))
		start_council(&settings);
	
	pthread_mutex_init(&mutex, NULL);
	
	// malloc philosophers needed
	// 1: alloc memory to each philosopher
	i = 0;
	while (i < settings.number_of_philosophers)
	{
		if (pthread_create(philosophers[i], NULL, &routine, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
		i++;
		
		if (pthread_join(philosophers[i], NULL) != 0)
			return (1);
	}
	
	pthread_mutex_destroy(&mutex);
}


