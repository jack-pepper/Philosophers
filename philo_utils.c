/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:10:35 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/16 15:23:15 by mmalie           ###   ########.fr       */
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
