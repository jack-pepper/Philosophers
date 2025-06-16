/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:58 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/16 15:09:16 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	ft_are_pos_int_strs(char **strs)
{
	int			i;
	int			j;
	long long	num;

//        if (!*strs || !strs)
//              return (0);
	i = 1;
	while (strs[i] != NULL)
	{
		j = 0;
		while (strs[i][j] != '\0')
		{
			if (!ft_isdigit(strs[i][j])) // SHOULD HANDLE SPACES ETC< TOO!
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

int	are_args_valid(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Wrong number of arguments (req: 5 or 6, found %d)\n", argc);
		return (-1);
	}
	if (ft_are_pos_int_strs(argv) != 0)
	{
		printf("Invalid arguments (req: only digits and positive int)\n");
		return (-1);
	}
	if (are_handable_settings(argv) != 0)
	{
		printf("Invalid arguments: need positive int numbers only\n");
		return (-1);
	}
	return (0);
}

// NB: will need a ft_atouint_64_t function for storing timestamps
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

/*
// Trim each arg (remove + sign and leading zeros)
char	*ps_trim(char *trimmed_arg, char *arg, int len, int k)
{
	int	i;

	i = 0;
	if (ft_strnopbrk(arg, "0") == NULL || ft_strnopbrk(arg, "-0") == NULL
		|| ft_strnopbrk(arg, "+0") == NULL)
	{
		trimmed_arg = (char *)malloc(1 + 1);
		if (!trimmed_arg)
			return (NULL);
		ft_strlcpy(trimmed_arg, "0", 1);
		return (trimmed_arg);
	}
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i] == '0')
		i++;
	trimmed_arg = (char *)malloc((len - i + (arg[0] == '-') + 1));
	if (!trimmed_arg)
		return (NULL);
	if (arg[0] == '-')
		trimmed_arg[k++] = '-';
	while (i < len)
		trimmed_arg[k++] = arg[i++];
	trimmed_arg[k] = '\0';
	return (trimmed_arg);
}

// Trim all args
char	**ps_trim_all(char **trimmed_args, char **args, int nb_args, int i)
{
	char	*trimmed_arg;
	int		k;
	size_t	arg_len;

	k = 0;
	while (i < nb_args)
	{
		if ((args[i][(int)ft_strlen(args[i]) - 1] == '-')
			|| (args[i][(int)ft_strlen(args[i]) - 1] == '+'))
			return (NULL);
		trimmed_arg = NULL;
		arg_len = ft_strlen(args[i]);
		trimmed_arg = ps_trim(trimmed_arg, args[i], arg_len, 0);
		if (!trimmed_arg)
			return (NULL);
		trimmed_args[k] = ft_strdup(trimmed_arg);
		free(trimmed_arg);
		if (!trimmed_args[k])
			return (NULL);
		i++;
		k++;
	}
	trimmed_args[k] = NULL;
	return (trimmed_args);
}


int	parse_args(int nb_elem, char **args, int i, char ***trimmed_args)
{
	if (*args == NULL)
		return (1);
	*trimmed_args = (char **)malloc(sizeof(char *) * (nb_elem + 1));
	if (!trimmed_args)
		return (1);
	if ((ps_trim_all(*trimmed_args, args, nb_elem, i) == NULL)
		|| (args_are_all_int(nb_elem, *trimmed_args) != 0)
		|| (args_has_no_duplicate(nb_elem, *trimmed_args, 0) != 0))
	{
		ps_free_all(*trimmed_args);
		return (1);
	}
	return (0);
}

// Used to check int's bounds
long long	ps_atoll(const char *nptr)
{
	long long	result;
	int			sign;

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

// Return 0 if args are all int, else 1.
int	args_are_all_int(int nb_elem, char **args)
{
	char	*unsigned_val;
	int		i;

	i = 0;
	while (i < nb_elem)
	{
		unsigned_val = &args[i][0];
		if (args[i][0] == '-')
			unsigned_val = &args[i][1];
		if ((ft_strnopbrk(unsigned_val, "0123456789") != NULL))
			return (1);
		if (ft_strlen(args[i]) >= 10)
		{
			if ((ps_atoll(args[i]) < -2147483648)
				|| (ps_atoll(args[i]) > 2147483647))
				return (1);
		}
		i++;
	}
	return (0);
}
    */
