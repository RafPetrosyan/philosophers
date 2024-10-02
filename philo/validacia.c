/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validacia.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raf <raf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:48:50 by rafpetro          #+#    #+#             */
/*   Updated: 2024/10/03 00:41:48 by raf              ###   ########.fr       */
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

int	error_handler(int i, t_philo_info *philo_info)
{
	if (i == 1)
	{
		printf("Memory allocation error!\n");
		free(philo_info);
		return (1);
	}
	if (i == 2)
	{
		printf("Memory allocation error!\n");
		free(philo_info->forks_arr);
		free(philo_info);
		return (1);
	}
	return (0);
}

int	check_dead(t_philo_info *philos)
{
	int	i;

	i = 0;
	while (i < philos->philos_count)
	{
		pthread_mutex_lock(&(philos->philos_arr[i].after_last_meal_mutex));
		if ((get_time() - philos->philos_arr[i].after_last_meal)
			>= (size_t) philos->time_to_die)
		{
			pthread_mutex_lock(&(philos->finish_mutex));
			philos->finish_time = 1;
			pthread_mutex_unlock(&(philos->finish_mutex));
			pthread_mutex_lock(&(philos->print_mutex));
			printf("%llu %d %s\n", get_time() - philos->start_time,
				i + 1, "died");
			pthread_mutex_unlock(&(philos->print_mutex));
			pthread_mutex_unlock(
				&(philos->philos_arr[i].after_last_meal_mutex));
			return (1);
		}
		pthread_mutex_unlock(&(philos->philos_arr[i].after_last_meal_mutex));
		++i;
	}
	return (0);
}

int	check_eaten(t_philo_info *pinfo)
{
	int	i;
	int	all_philos_get_finish_time;

	if (pinfo->count_eat == -1)
		return (0);
	i = 0;
	all_philos_get_finish_time = 0;
	while (i < pinfo->philos_count)
	{
		pthread_mutex_lock(&(pinfo->philos_arr[i].count_he_ate_m));
		if (pinfo->philos_arr[i].number_of_times_he_ate
			>= pinfo->count_eat)
			++all_philos_get_finish_time;
		pthread_mutex_unlock(&(pinfo->philos_arr[i].count_he_ate_m));
		++i;
	}
	if (all_philos_get_finish_time == pinfo->philos_count)
	{
		pthread_mutex_lock(&(pinfo->finish_mutex));
		pinfo->finish_time = 1;
		pthread_mutex_unlock(&(pinfo->finish_mutex));
		return (1);
	}
	return (0);
}
