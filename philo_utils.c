/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:10:35 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/16 18:18:15 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	display_settings(const t_settings *settings)
{
	printf("[display_settings]:\n");
	printf("number of philosophers: %d\n",
		settings->number_of_philosophers);
	printf("time to die: %d\n",
		settings->time_to_die);
	printf("time to eat: %d\n",
		settings->time_to_eat);
	printf("time to sleep: %d\n",
		settings->time_to_sleep);
	printf("number of times each philosopher must eat: %d\n",
		settings->number_of_times_each_philosopher_must_eat);
	return ;
}

uint64_t	get_timestamp_ms(struct timeval *tv)
{
	if (gettimeofday(&(*tv), NULL) != 0)
		printf("[get_timestamp_ms] gettimeofday fail\n");
	return (convert_to_ms(*tv));
}

uint64_t	convert_to_ms(struct timeval time)
{
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_ret(int return_val, char *error_msg)
{
	printf("%s", error_msg);
	return (return_val);
}
