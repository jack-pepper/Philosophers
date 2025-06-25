/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:32:52 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/25 22:08:00 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define DEBUG 0

# define STDOUT 1
# define STDERR 2

# define EXIT_ERR 1
# define EXIT_GRIEF 2
# define EXIT_SATIETY 3

# define MIN_THREADS 1
# define MAX_THREADS 200
# define MIN_TIME_MS 60
# define MAX_TIME_MS 1000000
# define MIN_MEALS 1
# define MAX_MEALS 1000000

/* Status messages */
# define FORK_MSG "has taken a fork"
# define EAT_MSG "is eating"
# define SLEEP_MSG "is sleeping"
# define THINK_MSG "is thinking"
# define DIED_MSG "died"

/* Error and success messages */

	// philo.c
# define ERR_INITER "\n❌ [main] err: initer\n"
# define ERR_LAUNCH_SIMULATION "\n❌ [main] err: launch_simulation\n"

# define SUC_INITER "\n🚀🐣 [main] INITER COMPLETE!\n"
# define SUC_LAUNCH_SIMULATION "\n🚀🥽 [launching_simulation] Let's go!\n"

# define EXIT_MESSAGE "\n╰┈➤🚪 [main] Freeing & exiting program.\n"

# define ERR_STATE "\n❌ [initer] err: state\n"
# define ERR_INIT_BARRIER "\n❌ [initer] err: init_barrier\n"
# define ERR_INIT_FORKS "\n❌ [initer] err: init_forks\n"
# define ERR_INIT_PHILOSOPHERS "\n❌ [initer] err: init_philosophers\n"
# define ERR_INIT_MUTEXES "\n❌ [initer] err: init_mutexes\n"

# define SUC_INIT_BARRIER "\n🚀🚧 [init_barrier] 👍 barrier ready!\n"
# define SUC_INIT_FORKS "\n🚀🍴 [init_forks] 👍 forks ready!\n"
# define SUC_INIT_PHILOSOPHERS "\n🚀👴 [init_philosophers] 👍 philo ready!\n"
# define SUC_INIT_MUTEXES "\n🚀🔐 [init_mutexes] 👍 mutexes ready!\n"

# define ERR_LAUNCH_DEATH_CLOCK "\n❌ Err: launch_death_clock\n"
# define ERR_CREATE_PHILO_THREADS "\n❌ Err: create_philo_threads\n"
# define ERR_JOIN_CLOCK_THREAD "\n❌ Err: pthread_join(clock.thread)\n"
# define ERR_JOIN_PHILO_THREADS "\n❌ Err: in join_philo_threads!\n"

# define SUC_LAUNCH_DEATH_CLOCK "\n	✅ Death clock launched!\n"
# define SUC_CREATE_PHILO_THREADS "\n	✅ Philo threads created!\n"
# define SUC_JOIN_CLOCK_THREAD "\n        ✅ Thread clock terminated!\n"
# define SUC_JOIN_PHILO_THREADS "\n	✅ Philo threads all terminated!\n"

# define ERR_MTX_INIT_DISPLAY_STATUS "❌ Err: mutex_init(display_status)\n"
# define ERR_MTX_INIT_GET_TIME "❌ Err: mutex_init(get_time)\n"
# define ERR_MTX_INIT_SIM_STATE "❌ Err: mutex_init(sim_state)\n"
# define ERR_MTX_INIT_THREADS_READY "❌ Err: mutex_init(threads_ready)\n"
# define ERR_MTX_INIT_PHILO_ALL_SET "❌ Err: mutex_init(philo_all_set)\n"
# define ERR_MTX_INIT_MEALS "❌ Err: mutex_init(meals)\n"
# define ERR_MTX_INIT_PHILO_ALL_FED_UP "❌ Err: mutex_init(philo_all_fed_up)\n"

// Complete with a strjoin
# define ERR_MTX_INIT_FORK "❌ Err: mutex_init(mtx_fork)\n"
# define ERR_MTX_INIT_IS_TAKEN "❌ Err: on mutex_init(is_taken): fork\n"
# define ERR_MTX_INIT_HAS_LEFT_FORK "❌ Err: mutex_init(has_left_work)\n"
# define ERR_MTX_INIT_HAS_RIGHT_FORK "❌ Err: mutex_init(has_right_fork)\n"

# define ERR_MTX_DEST_DISPLAY_STATUS "❌ Err: mutex_destroy(display_status)\n"
# define ERR_MTX_DEST_GET_TIME "❌ Err: mutex_destroy(get_time)\n"
# define ERR_MTX_DEST_SIM_STATE "❌ Err: mutex_destroy(sim_state)\n"
# define ERR_MTX_DEST_THREADS_READY "❌ Err: mutex_destroy(threads_ready)\n"
# define ERR_MTX_DEST_PHILO_ALL_SET "❌ Err: mutex_destroy(philo_all_set)\n"
# define ERR_MTX_DEST_BARRIER "❌ Err: mutex_destroy(break_through)\n"

# define ERR_MTX_DEST_FORK "❌ Err: mutex_destroy(mtx_fork)\n"
# define ERR_MTX_DEST_MEALS "❌ Err: mutex_destroy(mtx_meals)\n"
# define ERR_MTX_DEST_IS_TAKEN "❌ Err: mutex_destroy(is_taken)\n"
# define ERR_MTX_DEST_HAS_LEFT_FORK "❌ Err: mutex_destroy(has_left_work)\n"
# define ERR_MTX_DEST_HAS_RIGHT_FORK "❌ Err: mutex_destroy(has_right_fork)\n"

# define ERR_PHILO_THREAD_JOIN "❌ Err: philo thread not joined\n"

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
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_philosophers;
	int			number_of_times_each_philosopher_must_eat;
	int			margin;
}				t_settings;

typedef struct s_philosopher
{
	pthread_t			thread;
	struct s_philo_arg	*arg;
	pthread_mutex_t		mtx_has_left_fork;
	pthread_mutex_t		mtx_has_right_fork;
	pthread_mutex_t		mtx_meals;
	struct timeval		cur_time;
	uint64_t			cur_time_ms;
	struct timeval		last_meal_time;
	uint64_t			last_meal_time_ms;
	int					id;
	int					meals_eaten;
	bool				is_alive;
	bool				has_left_fork;
	bool				has_right_fork;
}						t_philosopher;

typedef struct s_fork
{
	pthread_mutex_t		mtx_fork;
	pthread_mutex_t		mtx_is_taken;
	int					id;
	bool				is_already_taken;
}						t_fork;

typedef struct s_clock
{
	pthread_t			thread;
	pthread_mutex_t		mtx_get_time;
	struct timeval		start_time;
	struct timeval		cur_time;
	volatile uint64_t	start_time_ms;
	volatile uint64_t	cur_time_ms;
}						t_clock;

typedef struct s_barrier
{
	pthread_mutex_t		mtx_barrier;
	int					threshold;
}						t_barrier;

typedef struct s_state
{
	pthread_mutex_t		mtx_sim_state;
	pthread_mutex_t		mtx_display_status;
	pthread_mutex_t		mtx_threads_ready;
	pthread_mutex_t		mtx_philo_all_set;
	pthread_mutex_t		mtx_philo_all_fed_up;
	t_settings			settings;
	t_philosopher		*philosophers;
	t_fork				*forks;
	t_clock				clock;
	t_barrier			barrier;
	int					threads_ready;
	int					current_i;
	volatile bool		simulation_on;
	volatile bool		philo_all_set;
	volatile bool		philo_all_fed_up;
	volatile bool		dead_spotted;
}						t_state;

typedef struct s_philo_arg
{
	t_state		*state;
	int			i;
	int			nb_guests;
}				t_philo_arg;

// philo.c
int			main(int argc, char **argv);
int			initer(t_state *state, int nb_guests);
int			launch_simulation(t_state *state, int nb_guests);

// args_parser.c
int			are_args_valid(int argc, char **argv);
void		store_args(char **argv, t_settings *settings);
int			ft_are_pos_int_strs(char **strs);
int			are_handable_settings(char **argv);
long long	ft_atoll(const char *nptr);

// initer.c
int			init_barrier(t_state *state);
int			init_forks(t_state *state, int nb_guests);
int			init_philosophers(t_state *state, int nb_guests);
int			init_mutexes(t_state *state, int nb_guests);

// mtx_handler.c
int			init_fork_mutexes(t_state *state, int nb_guests, int *i);
int			init_meals_mutexes(t_state *state, int nb_guests, int *i);
int			clean_ret(t_state *state, char *err_msg, int *i, int lvl);
int			clean_all_forks_mutexes(pthread_mutex_t *mtx, int *i);

// sim_launcher.c
int			launch_death_clock(t_state *state);
int			create_philo_threads(t_state *state, int nb_guests);
t_philo_arg	*set_philo_arg(t_state *state, t_philo_arg *arg,
				int i, int nb_guests);
int			join_philo_threads(t_state *state, int nb_guests);
void		handle_pthread_create_fail(t_state *state, t_philo_arg *arg);

// sim_state.c
void		set_sim_status(t_state *state, bool sim_status);
bool		is_sim_on(t_state *state);
void		set_philo_threads_all_set(t_state *state, bool set_status);
int			are_philo_threads_all_set(t_state *state);
int			are_philo_all_fed_up(t_state *state);

// gandalf_barrier.c
int			gandalf_barrier(t_state *state);
int			wait_philo_all_set(t_state *state);
int			wait(t_state *state);
int			break_through(t_state *state);
void		set_all_last_meal_time(t_state *state, int nb_guests);

// routine_clock.c
void		*clock_routine(void *arg);
void		toll_the_bell(t_state *state);
int			take_pulse(t_state *state, uint64_t timestamp_ms);
uint64_t	calc_starvation_duration(t_state *state, int i);
bool		verify_satiety(t_state *state, int nb_guests, int satiety);

// status_change.c
int			change_status(t_state *state, uint64_t timestamp_ms,
				t_philosopher *philosopher, char *status);
int			check_actions(t_state *state, uint64_t timestamp_ms,
				t_philosopher *philosopher, char *status);
void		die(t_state *state, t_philosopher *philosopher);
int			eat(t_state *state, t_philosopher *philosopher);
void		nap(t_state *state);

// routine_philo.c
void		*philo_routine(void *arg);
int			philo_routine_starter(t_philo_arg *this_arg, int i);
int			wait_sim_start(t_state *state);
int			have_council(t_state *state, int i, int next_i);
void		wait_for_death(t_philo_arg *this_arg, int i, int next_i);

// forks_algorithm.c
int			is_endcase_met(t_state *state);
int			wait_forks(t_state *state, uint64_t timestamp_ms,
				int i, int next_i);
int			left_handed_case(t_state *state, uint64_t timestamp_ms,
				int i, int next_i);
void		drop_forks(t_state *state, t_philosopher *philosopher, int i);
void		change_has_fork(t_state *state, int i, char *side, bool has_fork);

// fork_utils.c
int			take_left_fork(t_state *state, int i);
int			take_right_fork(t_state *state, int i, int next_i);
int			put_left_fork(t_state *state, int i);
int			put_right_fork(t_state *state, int i, int next_i);
int			change_fork_status(t_state *state, int i, bool is_taken);

// philo_actions.c
uint64_t	calc_timestamp_ms(t_state *state, int i);
int			check_exit_case(int res);
int			eat_pasta(t_state *state, uint64_t timestamp_ms, int i, int next_i);
int			take_a_nap(t_state *state, uint64_t timestamp_ms, int i);
int			think(t_state *state, uint64_t timestamp_ms, int i);

// end_cases.c
void		endcase_die_alone(t_state *state, t_philosopher *philosopher,
				int i);
void		endcase_agony(t_state *state, t_philosopher *philosopher);
void		endcase_grief(t_state *state, t_philosopher *philosopher, int i);
void		endcase_satiety(t_state *state, t_philosopher *philosopher, int i);
void		endcase_msg(t_philosopher *philo, char *endcase);

// memory.c
int			free_on_exit(t_state *state, int ret_val);
int			free_forks(t_state *state);
void		free_philosophers(t_state *state);
void		free_philo_args(t_state *state, int i);

// libft_utils.c
int			ft_isspace(int c);
int			ft_isdigit(int c);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_atoi(const char *nptr);
size_t		ft_strlen(const char *s);

// philo_utils.c
void		display_settings(const t_settings *settings);
int			ft_ret(int return_val, char *error_msg, int fd);
void		ft_putstr_fd(char *s, int fd);
int			set_next_i(t_state *state, int i, int *next_i);
void		set_optimal_delay(t_state *state, int margin, int i);

// pthread_wrappers.c
void		ft_mutex_lock(pthread_mutex_t *mtx);
void		ft_mutex_unlock(pthread_mutex_t *mtx);
int			ft_mutex_destroy(pthread_mutex_t *mtx, char *err_msg);
bool		is_dead_spotted(t_state *state);
void		set_last_meal_time(t_state *state, t_philosopher *philosopher,
				uint64_t timestamp_ms);

// time_utils.c
void		set_start_time(t_state *state);
uint64_t	get_cur_time(t_state *state);
uint64_t	get_timestamp_ms(struct timeval *tv);
uint64_t	convert_to_ms(struct timeval tv);
void		ft_usleep(t_state *state, useconds_t usec, char *err_msg);

#endif
