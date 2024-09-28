/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:24:22 by rafpetro          #+#    #+#             */
/*   Updated: 2024/09/28 13:05:33 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	validacia(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) == -1)
		{
			printf("Invalid Argument !\n");
			return (1);
		}
		if (i != argc - 1 && ft_atoi(argv[i]) == 0)
		{
			printf("Invalid Argument !\n");
			return (1);
		}
		++i;
	}
	return (0);
}

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

void	create_threads(t_philo_info *pinfo)
{
	int	i;

	i = 0;
	pinfo->philos_arr = (t_philo *)malloc((pinfo)->philos_count * sizeof(t_philo));
	if (pinfo->philos_arr == 0)
		return ;
	while (i < pinfo->philos_count)
	{
		pinfo->philos_arr[i].data = pinfo;
		pinfo->philos_arr[i].index = i + 1;
		pinfo->philos_arr[i].after_last_meal = pinfo->start_time;
		pinfo->philos_arr[i].number_of_times_he_ate = 0;
		pthread_create(&(pinfo->philos_arr[i].thread_id), NULL,
			routine, &(pinfo->philos_arr[i]));
		++i;
	}
	i = 0;
	while (i < (pinfo->philos_count))
	{
		pthread_mutex_init(&(pinfo->philos_arr[i].after_last_meal_mutex), NULL);
		pthread_mutex_init(&(pinfo->philos_arr[i].number_of_times_he_ate_mutex), NULL);
		i++;
	}
}
