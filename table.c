#include "philo.h"

int	set_forks(t_state **state, int nb_guests)
{
    int	i;

    // malloc for forks
    (*state)->forks = malloc(sizeof(t_fork) * nb_guests);
    if (!(*state)->forks)
        return (-1);
    printf("[set_forks] forks successfully mallocated!");
    // init fork mutexes
    i = 0;
    while (i < nb_guests)
    {
        (*state)->forks[i].id = i + 1;
        (*state)->forks[i].is_already_taken = false;
        if (pthread_mutex_init(&(*state)->forks[i].mutex, NULL) != 0)
        {
            // free forks and mutexes
            return (-1);
        }
        i++;
    }
	printf("[set_forks] forks successfully mutexed!\n");
	return (0);
}

int	set_philosophers(t_state **state, int nb_guests)
{
	int	i;

	// malloc philosophers
	(*state)->philosophers = malloc(sizeof(t_philosopher) * (nb_guests));
	if (!(*state)->philosophers)
		return (-1);
	memset((*state)->philosophers, 0, sizeof(t_philosopher) * nb_guests);
	printf("[set_philosophers] philosophers successfully mallocated!\n");
	i = 0;
	while (i < nb_guests)
	{	
		printf("[0]");
		(*state)->current_i = i;
		printf("[1]");
		(*state)->philosophers[i].id = i + 1; 
		printf("[2]");
	 	(*state)->philosophers[i].last_meal_time_ms = (*state)->clock.cur_time_ms;	
		printf("[3]");
		if (pthread_create(&(*state)->philosophers[i].thread, NULL, &philo_routine, state) != 0)
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
	t_state	*state;	
	int	i;
	
	state = (t_state *)arg;
	i = state->current_i;
	
	while (1)
	{
	// try to take 'left' fork (with same id as philosopher)
	pthread_mutex_lock(&state->forks[i].mutex);
	state->forks[i].is_already_taken = true;
	// try to take 'right' fork (i + 1, or 1 if last philosopher of the circle)
	pthread_mutex_lock(&state->forks[i + 1].mutex);
	state->forks[i + 1].is_already_taken = true;
	
	// eat during the chosen time
	change_status(&state->philosophers[i], "is eating");
	
	// unlock only after finishing eating
	state->forks[i + 1].is_already_taken = false;
	pthread_mutex_unlock(&state->forks[i + 1].mutex);
	state->forks[i].is_already_taken = false;
	pthread_mutex_unlock(&state->forks[i].mutex);
	}
}

void	change_status(t_philosopher *philosopher, char *new_state)
{
	uint64_t timestamp_ms;

	timestamp_ms = 0;
	if (ft_strncmp(new_state, DIED_MSG, ft_strlen(DIED_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, new_state);
		// display message and end simulation
	}
	else if (ft_strncmp(new_state, FORK_MSG, ft_strlen(FORK_MSG)))
	{	
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, new_state);
		//
	}
	else if (ft_strncmp(new_state, EAT_MSG, ft_strlen(EAT_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, new_state);
		// launch a counter starting from the beginning of the meal
		// wait time_to_eat time
		// unlock the forks here?
	}
	else if (ft_strncmp(new_state, SLEEP_MSG, ft_strlen(SLEEP_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, new_state);
		//
	}
	else if (ft_strncmp(new_state, THINK_MSG, ft_strlen(THINK_MSG)))
	{
		printf("%lu %d %s\n", timestamp_ms, philosopher->id, new_state);
		/* code */
	}

	else
	{
		printf("[change_status] Wrong message!\n");
		return ;
	}
}
