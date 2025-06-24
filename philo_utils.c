/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:10:35 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/24 23:15:11 by mmalie           ###   ########.fr       */
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
//	printf("Optimal delay: `%d`\n", settings->optimal_delay);
	return ;
}

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

int	set_next_i(t_state *state, int i, int *next_i)
{
	if (i == state->settings.number_of_philosophers - 1)
		(*next_i) = 0;
	else
		(*next_i) = i + 1;
	return ((*next_i));
}

void	set_optimal_delay(t_state *state, int i)
{
	int		nb_philos;
	int		time_to_eat;

	nb_philos = state->settings.number_of_philosophers;
	time_to_eat = state->settings.time_to_eat;
	if (nb_philos < 10)
	{
		if (i % 2 == 1)
			ft_usleep(state, (useconds_t)(time_to_eat / 2),
				"[set_optimal_delay] usleep failed\n");
	}
	else if (nb_philos < 50)
	{
		if (i % 2 == 1)
			ft_usleep(state, (useconds_t)(time_to_eat / 2),
				"[set_optimal_delay] usleep failed\n");
		//else if (i % 3 == 0)
		//	ft_usleep(state, (useconds_t)(time_to_eat / 4),
		//		"[set_optimal_delay] usleep failed\n");
	}
	else if (nb_philos >= 50)
	{
		ft_usleep(state, (useconds_t)((time_to_eat * i) / nb_philos),
				"[set_optimal_delay] usleep failed\n");
	}
}
