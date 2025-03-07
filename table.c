#include "philo.h"

int	set_forks(t_state **state, int nb_guests)
{
    int	i;

	printf("[set_forks] mallocating for forks...\n");
    // malloc for forks
    (*state)->forks = malloc(sizeof(t_fork) * nb_guests);
    if (!(*state)->forks)
        return (-1);
    printf("[set_forks] forks successfully mallocated!\n");
    // init fork mutexes
	printf("[set_forks] initiating forks' mutexes...\n");
    i = 0;
    while (i < nb_guests)
    {
        (*state)->forks[i].id = i + 1;
        (*state)->forks[i].is_already_taken = false;
        if (pthread_mutex_init(&(*state)->forks[i].mutex, NULL) != 0)
        {
            // free forks and mutexes
	    printf("[set_forks] Issue while initiating forks[%d] mutex\n", i);
            return (-1);
        }
        i++;
    }
	printf("[set_forks] forks successfully mutexed!\n");
	return (0);
}

int	set_philosophers(t_state **state, int nb_guests)
{
	t_philo_arg	*arg;	
	int			i;

	// malloc and init philosophers	
	printf("[set_philosophers] mallocating for %d philosophers...\n", nb_guests);
	(*state)->philosophers = malloc(sizeof(t_philosopher) * (nb_guests));
	if (!(*state)->philosophers)
		return (-1);
	memset((*state)->philosophers, 0, sizeof(t_philosopher) * nb_guests);
	printf("[set_philosophers] philosophers successfully mallocated!\n");
	
	// set data
	i = 0;
	while (i < nb_guests)
	{	
		arg = malloc(sizeof(arg));
		if (!arg)
			return (-1);
		//memset(arg, 0, sizeof(t_philo_arg));
		printf("[0]");
		arg->state = &(*state);
		printf("[1]");
		arg->this_philosopher = &(*state)->philosophers[i]; 
		printf("[2]");
	// 	(*state)->philosophers[i].last_meal_time_ms = (*state)->clock.cur_time_ms;	
		printf("[3]");
		if (pthread_create(&(*state)->philosophers[i].thread, NULL, &philo_routine, arg) != 0)
		{
			// free memory depending of i
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < nb_guests)
	{
		if (pthread_join((*state)->philosophers[i].thread, NULL) != 0)
		{
			// free memory depending of i
			return (1);
		}
		i++;
	}
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo_arg	*this_arg;
	int	i;
	
	this_arg = (t_philo_arg *)arg;
	i = this_arg->this_philosopher->id;
	printf("[philo_routine] current_i: %d\n", this_arg->this_philosopher->id);
	
	while (1)
	{
		// try to take 'left' fork (with same id as philosopher)
		//printf("Trying to take left fork...\n");
		pthread_mutex_lock(&(*this_arg->state)->forks[i].mutex);
		change_status(this_arg->state, &(*this_arg->state)->philosophers[i], FORK_MSG);
		(*this_arg->state)->forks[i].is_already_taken = true;
		// try to take 'right' fork (i + 1, or 1 if last philosopher of the circle)
		//printf("Trying to take right fork...\n");
		pthread_mutex_lock(&(*this_arg->state)->forks[i + 1].mutex);
		change_status(this_arg->state, &(*this_arg->state)->philosophers[i], FORK_MSG);
		(*this_arg->state)->forks[i + 1].is_already_taken = true;
		// eat during the chosen time
		change_status(this_arg->state, &(*this_arg->state)->philosophers[i], EAT_MSG);
	
		// unlock only after finishing eating
		//printf("Put right fork down\n");
		(*this_arg->state)->forks[i + 1].is_already_taken = false;
		pthread_mutex_unlock(&(*this_arg->state)->forks[i + 1].mutex);
		//printf("Put left fork down\n");
		(*this_arg->state)->forks[i].is_already_taken = false;
		pthread_mutex_unlock(&(*this_arg->state)->forks[i].mutex);
	}
}

void	change_status(t_state **state, t_philosopher *philosopher, char *status)
{
	uint64_t timestamp_ms;

	timestamp_ms = (*state)->clock.cur_time_ms;
	if (ft_strncmp(status, DIED_MSG, ft_strlen(DIED_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		// display message and end simulation
	}
	else if (ft_strncmp(status, FORK_MSG, ft_strlen(FORK_MSG)))
	{	
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		//
	}
	else if (ft_strncmp(status, EAT_MSG, ft_strlen(EAT_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		// launch a counter starting from the beginning of the meal
		// wait time_to_eat time
		// unlock the forks here?
	}
	else if (ft_strncmp(status, SLEEP_MSG, ft_strlen(SLEEP_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		//
	}
	else if (ft_strncmp(status, THINK_MSG, ft_strlen(THINK_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, status);
		/* code */
	}

	else
	{
		printf("[change_status] Wrong message!\n");
		return ;
	}
}
