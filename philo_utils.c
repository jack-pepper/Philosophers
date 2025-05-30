/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:10:35 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/31 00:25:02 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	display_settings(const t_settings *settings)
{
	printf("\n⚙️  [SETTINGS] ⚙️\n");
	printf("\n👴 number of philosophers: `%d`\n",
		settings->number_of_philosophers);
	printf("⏱️ 💀  time to die: `%d`\n",
		settings->time_to_die);
	printf("⏱️ 🍴 time to eat: `%d`\n",
		settings->time_to_eat);
	printf("⏱️ 💤 time to sleep: `%d`\n",
		settings->time_to_sleep);
	printf("🎯 number of times each philosopher must eat: `%d`\n",
		settings->number_of_times_each_philosopher_must_eat);
	return ;
}

//int	ft_ret(int return_val, char *error_msg)
//{
//	printf("%s", error_msg);
//	return (return_val);
//}

int	ft_ret(int return_val, char *msg, int fd)
{
	ft_putstr_fd(msg, fd);
	return (return_val);
}

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	if (s != NULL)
	{
		i = 0;
		while (s[i] != '\0')
		{
			write(fd, &s[i], 1);
			i++;
		}
	}
}

void	change_has_fork(t_state *state, int i, char *side, bool has_fork)
{
	if (ft_strcmp(side, "left") == 0)
	{
		pthread_mutex_lock(&(state)->philosophers[i].mtx_has_left_fork);
		state->philosophers[i].has_left_fork = has_fork;
		pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_left_fork);
	}
	else if (ft_strcmp(side, "right") == 0)
	{
		pthread_mutex_lock(&(state)->philosophers[i].mtx_has_right_fork);
        	state->philosophers[i].has_right_fork = has_fork;	
		pthread_mutex_unlock(&(state)->philosophers[i].mtx_has_right_fork);
	}
}

uint64_t        calc_starvation_duration(t_state *state, int i)
{
        uint64_t        starving_since;

        pthread_mutex_lock(&(state->clock.mtx_get_time));
        starving_since = state->clock.cur_time_ms
                - state->philosophers[i].last_meal_time_ms;
        pthread_mutex_unlock(&(state->clock.mtx_get_time));
        return (starving_since);
}
