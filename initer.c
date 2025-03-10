#include "philo.h"

int	init_forks(t_state *state, int nb_guests)
{
	int	i;

	printf("[init_forks] mallocating forks for %d guests...\n", nb_guests);
	state->forks = malloc(sizeof(t_fork) * nb_guests);
	if (!state->forks)
		return (-1);
	memset(state->forks, 0, sizeof(t_fork) * nb_guests);
	printf("[init_forks] forks successfully mallocated and initialized!\n");
	i = 0;
	while (i < nb_guests)
	{
		state->forks[i].id = i + 1;
		state->forks[i].is_already_taken = false;
		i++;
	}
	printf("[init_forks] forks successfully initiated (id, is_already_taken)!\n");
	return (0);
}

int	init_philosophers(t_state *state, int nb_guests)
{
	int                     i;

	printf("[init_philosophers] mallocating for %d philosophers...\n", nb_guests);
	state->philosophers = malloc(sizeof(t_philosopher) * (nb_guests));
	if (!state->philosophers)
		return (-1);
	memset(state->philosophers, 0, sizeof(t_philosopher) * nb_guests);
	printf("[init_philosophers] philosophers successfully mallocated and initialized!\n");
	i = 0;
	while (i < nb_guests)
	{
		state->philosophers[i].id = i + 1;
		i++;
	}
	return (0);
}

/*
int     init_clock(t_state *state)
{
        if (gettimeofday(&(*state)->clock.start_time, NULL) != 0)
                printf("[init_clock] gettimeofday fail\n");
        if (gettimeofday(&(*state)->clock.cur_time, NULL) != 0)
                printf("[init_clock] gettimeofday fail\n");
        state->clock.start_time_ms = convert_to_ms(state->clock.start_time);
        state->clock.cur_time_ms = convert_to_ms(state->clock.cur_time);
        printf("[init_clock] %lu\n", state->clock.start_time_ms);
        return (0);
}
*/

int	init_mutexes(t_state *state, int nb_guests)
{
	int	i;

	printf("[init_mutexes] initiating forks' mutexes...\n");
	i = 0;
	while (i < nb_guests)
	{
		//state->forks[i].id = i + 1;
		//state->forks[i].is_already_taken = false;
		if (pthread_mutex_init(&state->forks[i].mutex, NULL) != 0)
		{
			// free forks and mutex
			printf("[init_mutexes] Issue while initiating forks[%d] mutex\n", i);
			return (-1);
		}
		i++;
	}
	printf("[init_mutexes] forks successfully mutexed!\n");
	if (pthread_mutex_init(&state->clock.mutex_get_time, NULL) != 0)
	{
		// free forks and mutexe
		printf("[init_mutexes] Issue while initiating clock.mutex_get_time\n");
            	return (-1);
        }
	return (0);
}
