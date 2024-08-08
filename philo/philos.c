/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:24:17 by rafpetro          #+#    #+#             */
/*   Updated: 2024/08/08 16:40:57 by rafpetro         ###   ########.fr       */
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
	init_philo_info(philo_info, argc, argv);
	philo_info->forks_arr = malloc(philo_info->philos_count	* sizeof(pthread_mutex_t));
	if (philo_info->forks_arr == 0)
	{
		free(philo_info);
		return (printf("Memory allocation error!\n"));
	}
	init_mutex(philo_info);
	create_threads(philo_info);
	while (1)
	{
		if (check_dead(philo_info))
			break ;
		if (check_eaten(philo_info))
			break ;
	}
	close_destroy(philo_info);
	return (0);
}

void	init_philo_info(t_philo_info **philo_info, int argc, char **argv)
{
	int	i;
	
	(*philo_info)->start_time = get_time();
	(*philo_info)->philos_count = ft_atoi(argv[1]);
	(*philo_info)->time_to_die = ft_atoi(argv[2]);
	(*philo_info)->time_to_eat = ft_atoi(argv[3]);
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
		pthread_mutex_init(&(philo_info->philos_arr[i].number_of_times_he_ate_mutex), NULL);
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
		pthread_mutex_lock(&(philo->after_last_meal_mutex));
		philo->after_last_meal = get_time();
		pthread_mutex_unlock(&(philo->after_last_meal_mutex));
		ft_usleep(philo->data->time_to_sleep, philo->data);
		print(philo->data, philo->index, "is thinking");
	}
	return (0);
}

void	init_mutex(t_philo_info *philo_info)
{
	int	i;

	pthread_mutex_init(&(philo_info->print_mutex), NULL);
	pthread_mutex_init(&(philo_info->finish_mutex), NULL);
	i = 0;
	while (i < (philo_info->philos_count))
	{
		pthread_mutex_init(&(philo_info->forks_arr[i]), NULL);
		i++;
	}
}
int	eating(t_philo *philo, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork)
{
	pthread_mutex_lock(right_fork);
	print(philo->data, philo->index, "has taken a fork");
	if (philo->data->philos_count == 1)
	{
		pthread_mutex_unlock(right_fork);
		return (0);
	}
	pthread_mutex_lock(left_fork);
	print(philo->data, philo->index, "has taken a fork");
	print(philo->data, philo->index, "is eating");
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(right_fork);
	pthread_mutex_unlock(left_fork);
	pthread_mutex_lock(&(philo->number_of_times_he_ate_mutex));
	philo->number_of_times_he_ate++;
	pthread_mutex_unlock(&(philo->number_of_times_he_ate_mutex));
	return (1);
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

int	get_finish_time(t_philo_info *philos)
{
	int	finish_time;

	pthread_mutex_lock(&(philos->finish_mutex));
	finish_time = philos->finish_time;
	pthread_mutex_unlock(&(philos->finish_mutex));
	return (finish_time);
}