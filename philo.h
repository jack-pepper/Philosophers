/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:32:52 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/07 23:16:44 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <stdlib.h>
# include <unistd.h>
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
	pthread_mutex_t	mutex_start_simulation;
	t_settings		settings;
	t_philosopher	*philosophers;
	t_fork			*forks;
	t_clock			clock;
	int				current_i;
	bool			simulation_on;
}					t_state;

typedef struct		s_philo_arg
{
	t_state		*state;
	int		i;
	int		nb_guests;
}					t_philo_arg;


// philo.c
int	main(int argc, char **argv);
int     initer(t_state *state);
int     launch_simulation(t_state *state);
int     launch_death_clock(t_state *state, int nb_guests);

// args_parser.c
void    store_args(char **argv, t_settings *settings);

// initer.c
int     init_forks(t_state *state, int nb_guests);
int     init_philosophers(t_state *state, int nb_guests);
//int     init_clock(t_state *state);
int	init_mutexes(t_state *state, int nb_guests);

// routines.c
void    *clock_routine(void *arg);
void    *philo_routine(void *arg);
int         take_pulse(t_state *state);
void    change_status(t_state *state, t_philosopher *philosopher, char *status);

// memory.c
int    		free_on_exit(t_state *state);
int    		detach_threads(t_state *state);
int   		free_forks(t_state *state);
void   		free_philosophers(t_state *state);

// utils.c
uint64_t    convert_to_ms(struct timeval time);
int			ft_isspace(int c);
int			ft_isdigit(int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_atoi(const char *nptr);
size_t		ft_strlen(const char *s);

#endif
