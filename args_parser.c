/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:58 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/19 23:08:31 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	are_args_valid(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Invalid args: req 5 or 6, found %d!\n", argc);
		return (-1);
	}
	if (ft_are_pos_int_strs(argv) != 0)
	{
		printf("Invalid args: req only digits and positive int!\n");
		return (-1);
	}
	if (are_handable_settings(argv) != 0)
	{
		printf("Invalid args: performance concern!\n");
		return (-1);
	}
	return (0);
}

int	ft_are_pos_int_strs(char **strs)
{
	int			i;
	int			j;
	long long	num;

	if (!*strs || !strs)
		return (0);
	i = 1;
	while (strs[i] != NULL)
	{
		j = 0;
		while (strs[i][j] != '\0')
		{
			if (!ft_isdigit(strs[i][j]) && !ft_isspace(strs[i][j]))
				return (1);
			j++;
		}
		num = ft_atoll(strs[i]);
		if (num > INT_MAX)
			return (1);
		i++;
	}
	return (0);
}

int	are_handable_settings(char **argv)
{
	if (ft_atoi(argv[1]) < MIN_THREADS || ft_atoi(argv[1]) > MAX_THREADS)
	{
		printf("Err: number_of_philosophers (1): req: %d < argv[1] > %d\n",
			MIN_THREADS, MAX_THREADS);
		return (1);
	}
	if ((ft_atoi(argv[2]) < MIN_TIME_MS || ft_atoi(argv[2]) > MAX_TIME_MS)
		|| (ft_atoi(argv[3]) < MIN_TIME_MS || ft_atoi(argv[3]) > MAX_TIME_MS)
		|| (ft_atoi(argv[4]) < MIN_TIME_MS || ft_atoi(argv[4]) > MAX_TIME_MS))
	{
		printf("Err: time_to_die (2), time_to_eat (3) and time_to_sleep (4):");
		printf("req: %d < argv[i] > %d\n", MIN_TIME_MS, MAX_TIME_MS);
		return (1);
	}
	if (argv[5] && (ft_atoi(argv[5]) < MIN_MEALS
			|| ft_atoi(argv[5]) > MAX_MEALS))
	{
		printf("Err: number_of_times_each_philosopher_must_eat (5):");
		printf("req: %d < argv[5] > %d\n", MIN_MEALS, MAX_MEALS);
	}
	return (0);
}

long long	ft_atoll(const char *nptr)
{
	long long	result;
	long long	sign;

	result = 0;
	sign = 1;
	while (ft_isspace(*nptr))
	{
		nptr++;
	}
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
		{
			sign = -1;
		}
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		result = (result * 10) + (*nptr - '0');
		nptr++;
	}
	return (result * sign);
}

void	store_args(char **argv, t_settings *settings)
{
	settings->number_of_philosophers = ft_atoi(argv[1]);
	settings->time_to_die = ft_atoi(argv[2]);
	settings->time_to_eat = ft_atoi(argv[3]);
	settings->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		settings->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	return ;
}
