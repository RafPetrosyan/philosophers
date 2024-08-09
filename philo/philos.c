/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:24:17 by rafpetro          #+#    #+#             */
/*   Updated: 2024/08/09 11:28:21 by rafpetro         ###   ########.fr       */
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
		return (error_handler(1, philo_info));
	init_mutex(philo_info);
	philo_info->philos_arr = (t_philo *)malloc((philo_info)->philos_count * sizeof(t_philo));
	if (philo_info->philos_arr == 0)
		return (error_handler(2, philo_info));	
	create_threads(philo_info);
	while (1)
	{
		if (check_dead(philo_info) || check_eaten(philo_info))
			break ;
	}
	return (close_destroy(philo_info));
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


void	*routine(void *philo_void)
{
	t_philo			*philo;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;

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

int	close_destroy(t_philo_info *philos_info)
{
	int	i;

	i = 0;
	while (i < philos_info->philos_count)
	{
		pthread_join(philos_info->philos_arr[i].thread_id, NULL);
		pthread_mutex_destroy(&(philos_info->philos_arr[i]));
		pthread_mutex_destroy(&(philos_info->philos_arr[i].after_last_meal_mutex));
		pthread_mutex_destroy(
			&(philos_info->philos_arr[i].number_of_times_he_ate_mutex));
		i++;
	}
	pthread_mutex_destroy(&(philos_info->finish_mutex));
	pthread_mutex_destroy(&(philos_info->print_mutex));
	free(philos_info->philos_arr);
	free(philos_info->forks_arr);
	free(philos_info);
	return (0);
}
