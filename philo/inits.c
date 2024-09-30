/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:49:46 by rafpetro          #+#    #+#             */
/*   Updated: 2024/09/30 16:00:03 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	init_mutex(t_philo_info *philo_info)
{
	int	i;

	pthread_mutex_init(&(philo_info->print_mutex), NULL);
	pthread_mutex_init(&(philo_info->finish_mutex), NULL);
	i = 0;
	while (i < (philo_info->philos_count))
	{
		pthread_mutex_init(&(philo_info->forks_arr[i]), NULL);
		++i;
	}
}

void	init_philo_info(t_philo_info *philo_info, int argc, char **argv)
{
	philo_info->start_time = get_time();
	philo_info->philos_count = ft_atoi(argv[1]);
	philo_info->time_to_die = ft_atoi(argv[2]);
	philo_info->time_to_eat = ft_atoi(argv[3]);
	philo_info->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		philo_info->count_eat = ft_atoi(argv[1]);
	else
		philo_info->count_eat = -1;
	philo_info->finish_time = 0;
}

void	create_threads(t_philo_info *pinfo)
{
	int	i;

	i = 0;
	pinfo->philos_arr = malloc((pinfo)->philos_count * sizeof(t_philo));
	if (pinfo->philos_arr == 0)
		return ;
	while (i < pinfo->philos_count)
	{
		pinfo->philos_arr[i].data = pinfo;
		pinfo->philos_arr[i].index = i;
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
		pthread_mutex_init(
			&(pinfo->philos_arr[i].number_of_times_he_ate_mutex), NULL);
		i++;
	}
}
