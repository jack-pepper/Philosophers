#ifndef PHILO_H
# define PHILO_H

# define FORK_MSG "has taken a fork"
# define EAT_MSG "is eating"
# define SLEEP_MSG "is sleeping"
# define THINK_MSG "is thinking"
# define DIED_MSG "died"

# include <pthread.h>
# include <stdint.h>
# include <string.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/time.h>

typedef struct s_settings
{
    int		   time_to_die;
    int		   time_to_eat;
    int		   time_to_sleep;
	int        number_of_philosophers;
	int	       number_of_times_each_philosopher_must_eat;
}				t_settings;

typedef struct 		s_philosopher
{
	pthread_t		thread;
	struct timeval	last_meal_time;
	uint64_t		last_meal_time_ms;
	int				id;
	int				meals_eaten;
	int				is_dead;
	int				is_eating;
	int				is_sleeping;
	int				is_thinking;
}					t_philosopher;

typedef struct 		s_fork
{
	pthread_mutex_t mutex;
	int				id;
	bool			is_already_taken; // or only mutex?
}					t_fork;

typedef struct 		s_clock
{
	pthread_t		thread;
	struct timeval	start_time;
	struct timeval	cur_time;
	uint64_t		start_time_ms;
	uint64_t		cur_time_ms;
}					t_clock;

typedef struct 		s_state
{
	t_settings		settings;
	t_philosopher	*philosophers;
	t_fork			*forks;
	t_clock			clock;
	int				current_i;
}					t_state;

// philo.c
int		main(int argc, char **argv);
int		start_council(t_state *state, t_philosopher *philosophers, t_fork *forks);

// table.c
int		set_forks(t_fork *forks, int nb_guests);
int		set_philosophers(t_state *state, t_philosopher *philosophers, int nb_guests);
void	*philo_routine(t_state *state);
void	change_status(t_philosopher *philosopher, char *new_state);

// clock.c
int		set_clock(t_state *state);
void    *clock_routine(t_state *state);
int		take_pulse(t_state *state);


#endif