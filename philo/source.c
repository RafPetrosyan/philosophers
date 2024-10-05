/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:24:22 by rafpetro          #+#    #+#             */
/*   Updated: 2024/10/05 15:46:25 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	ft_atoi(const char *nptr)
{
	int		i;
	size_t	numb;

	i = 0;
	numb = 0;
	if (!(nptr[i] >= '0' && nptr[i] <= '9'))
		return (-1);
	if (nptr[i] == '0' && nptr[i + 1] != '\0')
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		numb = numb * 10 + nptr[i] - '0';
		++i;
	}
	if (nptr[i] != '\0')
		return (-1);
	if (i >= 11 || numb > 2147483647)
		return (-1);
	return (numb);
}

long long	get_time(void)
{
	struct timeval	time;
	static size_t	smt;

	if (smt == 0)
	{
		gettimeofday(&time, NULL);
		smt = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	}
	if (gettimeofday(&time, NULL))
		return (0);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000) - smt);
}

int	get_finish_time(t_philo_info *philo_info)
{
	int	finish_time;

	pthread_mutex_lock(&(philo_info->finish_mutex));
	finish_time = philo_info->finish_time;
	pthread_mutex_unlock(&(philo_info->finish_mutex));
	return (finish_time);
}

int	print(t_philo_info *philo_info, int index, char *message)
{
	if (get_finish_time(philo_info))
		return (0);
	pthread_mutex_lock(&(philo_info->print_mutex));
	printf("%llu %d %s\n", get_time() - philo_info->start_time,
		index + 1, message);
	pthread_mutex_unlock(&(philo_info->print_mutex));
	return (1);
}

void	ft_usleep(long long mls, t_philo_info *info)
{
	long long	t;

	t = get_time();
	while (get_time() - t <= mls)
	{
		if (get_finish_time(info))
			return ;
		usleep(500);
	}
}
