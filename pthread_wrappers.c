/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_wrappers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 07:44:14 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/19 18:04:43 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_mutex_lock(pthread_mutex_t *mtx)
{
	if (pthread_mutex_lock(mtx) != 0)
		printf("WARNING: Error on mutex_lock!\n");
	return ;
}

void	ft_mutex_unlock(pthread_mutex_t *mtx)
{
	if (pthread_mutex_unlock(mtx) != 0)
		printf("WARNING: Error on mutex_unlock!\n");
	return ;
}

int	ft_mutex_destroy(pthread_mutex_t *mtx, char *err_msg)
{
	int	res;

	res = 0;
	if (pthread_mutex_destroy(mtx) != 0)
	{
		ft_putstr_fd(err_msg, STDERR);
		res = 1;
	}
	return (res);
}
