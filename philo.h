#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdint.h>
# include <string.h>
# include <stdio.h>
# include <limits.h>

typedef struct s_philo_vars
{
    uint64_t   time_to_die;
    uint64_t   time_to_eat;
    uint64_t   time_to_sleep;
	int        number_of_philosophers;
	int	       number_of_times_each_philosopher_must_eat;
}		t_philo_vars;

typedef struct s_philosopher
{
	pthread_t	thread;
	int	id;
	int	meals_eaten;
	int	is_dead;
	int	is_eating;
	int	is_sleeping;
	int	is_thinking;
}		t_philosopher;

#endif