/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:24:17 by rafpetro          #+#    #+#             */
/*   Updated: 2024/08/06 18:22:54 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	main(int argc, char **argv)
{
	t_philo_info	*philo_info;
	
	if (!(argc == 5 || argc == 6))
		return (printf("Invalid number of arguments\n"));
	if (validacia(argc, argv) == 1)
		return (printf("invalid argument\n"));
	philo_info = (t_philo_info *)malloc(sizeof(t_philo_info));
	if (philo_info == 0)
		return (printf("Memory allocation error!\n"));
	init_philos(philo_info, argc, argv);
	init_mutex(philo_info);
}

void	init_philos(t_philo_info **philo_info, int argc, char **argv)
{
	int	i;
	
	(*philo_info)->start_time = get_time();/// 								get_time;
	(*philo_info)->philos_count = ft_atoi(argv[1]);
	(*philo_info)->time_to_die = ft_atoi(argv[2]);
	(*philo_info)->time_to_die = ft_atoi(argv[3]);
	(*philo_info)->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		(*philo_info)->count_eat = ft_atoi(argv[1]);
	else
		(*philo_info)->count_eat = -1;
	(*philo_info)->finish_time = 0;
	if ((*philo_info)->count_eat == 0)
		return 0;
}

void	create_threads(t_philo_info *philo_info)
{
	int	i;

	i = 0;
	philo_info->philos_arr = (t_philo *)malloc((philo_info)->philos_count * sizeof(t_philo));
	if (philo_info->philos_arr == 0)
		return ;
	while (i < philo_info->philos_count)
	{
		philo_info->philos_arr[i].data = philo_info;
		philo_info->philos_arr[i].index = i + 1;
		philo_info->philos_arr[i].after_last_meal = philo_info->start_time;
		philo_info->philos_arr[i].number_of_times_he_ate = 0;
		pthread_create(&(philo_info->philos_arr[i].thread_id), NULL,
			routine, &(philo_info->philos_arr[i]));
		++i;
	}
	i = 0;
	while (i < (philo_info->philos_count))
	{
		pthread_mutex_init(&(philo_info->philos_arr[i].after_last_meal_mutex), NULL);
		pthread_mutex_init(&(philo_info->philos_arr[i].number_of_times_he_ate_mutex),
			NULL);
		i++;
	}
}

void	*routine(void *philo_void)
{
	t_philo			*philo;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

	philo = philo_void;
	if (philo->index == 1)
		left_fork = &(philo->data->forks_arr[philo->data->philos_count - 1]);
	else
		left_fork = &(philo->data->forks_arr[philo->index - 1]);
	right_fork = &(philo->data->forks_arr[philo->index]);
	if ((philo->index) % 2 == 0)
		usleep(500);
	while (!finished(philo->data))
	{
		if (!eating(philo, left_fork, right_fork))
			return (0);
		print(philo->data, philo->index, "is sleeping");
	}
	return (0);
}

void	init_mutex(t_philo_info *philos)
{
	int	i;

	pthread_mutex_init(&(philos->print_mutex), NULL);
	pthread_mutex_init(&(philos->finish_mutex), NULL);
	philos->forks_arr = malloc(philos->philos_count	* sizeof(pthread_mutex_t));
	if (!philos->forks_arr)
		return ;
	i = 0;
	while (i < (philos->philos_count))
	{
		pthread_mutex_init(&(philos->forks_arr[i]), NULL);
		i++;
	}
}