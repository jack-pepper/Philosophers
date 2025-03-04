#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdint.h>
# include <string.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_settings
{
    int		   time_to_die;
    int		   time_to_eat;
    int		   time_to_sleep;
	int        number_of_philosophers;
	int	       number_of_times_each_philosopher_must_eat;
}				t_settings;

typedef struct 	s_philosopher
{
	pthread_t	thread;
	int			id;
	int			meals_eaten;
	int			is_dead;
	int			is_eating;
	int			is_sleeping;
	int			is_thinking;
}				t_philosopher;

typedef struct 		s_fork
{
	pthread_mutex_t mutex;
	int				id;
	bool			is_already_taken; // or only mutex?
}					t_fork;

typedef struct 		s_state
{
	t_settings		settings;
	t_philosopher	*philosophers;
	t_fork			*forks;
	int				current_i;
}					t_state;

// philo.c
int		main(int argc, char **argv);
int		start_council(t_state *state, t_philosopher *philosophers, t_fork *forks);
void	*routine(t_state *state);

// initer.c
int		set_forks(t_fork *forks, int nb_guests);
int		set_philosophers(t_state *state, t_philosopher *philosophers, int nb_guests);

//


#endif