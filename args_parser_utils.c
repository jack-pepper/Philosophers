/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:23:58 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/19 23:07:11 by mmalie           ###   ########.fr       */
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
