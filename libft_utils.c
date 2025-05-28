/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:10:35 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/28 23:00:50 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isspace(int c)
{
	return (c == 32 || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v');
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] == s2[i]) && s1[i] && s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		result = (result * 10) + (*nptr - '0');
		nptr++;
	}
	return (result * sign);
}

size_t	ft_strlen(const char *s)
{
	const char	*str;

	str = s;
	while (*str)
	{
		str++;
	}
	return (str - s);
}
