/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:24:17 by rafpetro          #+#    #+#             */
/*   Updated: 2024/10/05 16:05:19 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	main(int argc, char **argv)
{
	t_philo_info	*philo_info;

	if (!(argc == 5 || argc == 6))
		return (printf("\t! Error !\nInvalid number of arguments\n"));
	if (validacia(argc, argv) == 1)
		return (printf("    ! Error !\nInvalid arguments\n"));
	philo_info = malloc(sizeof(t_philo_info));
	if (philo_info == 0)
		return (printf("\t! Error !\nMemory allocation error!\n"));
	init_philo_info(philo_info, argc, argv);
	if (philo_info->count_eat == 0)
		return (0);
	philo_info->forks_arr = malloc(philo_info->philos_count
			* sizeof(pthread_mutex_t));
	if (philo_info->forks_arr == 0)
		return (error_handler(1, philo_info));
	init_mutex(philo_info);
	philo_info->philos_arr = malloc(philo_info->philos_count * sizeof(t_philo));
	if (philo_info->philos_arr == 0)
		return (error_handler(2, philo_info));
	create_threads(philo_info);
	while (1)
		if (check_dead(philo_info) || check_eaten(philo_info))
			break ;
	return (!(philo_info->philos_arr == 0 || !close_destroy(philo_info)));
}

void	*routine(void *philo_void)
{
	t_philo			*philo;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;

	philo = philo_void;
	if (philo->index == 0)
		left_fork = &(philo->data->forks_arr[philo->data->philos_count - 1]);
	else
		left_fork = &(philo->data->forks_arr[philo->index - 1]);
	right_fork = &(philo->data->forks_arr[philo->index]);
	if ((philo->index + 1) % 2 == 0)
		usleep(500);
	while (!get_finish_time(philo->data))
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

int	eating(t_philo *philo, pthread_mutex_t *left_fork,
	pthread_mutex_t *right_fork)
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
	pthread_mutex_lock(&(philo->count_he_ate_m));
	++philo->number_of_times_he_ate;
	pthread_mutex_unlock(&(philo->count_he_ate_m));
	return (1);
}

int	taking_forks(t_philo *philo,
	pthread_mutex_t **l_fork, pthread_mutex_t **r_fork)
{
	if (!print(philo->data, philo->index, "has taken a fork")
		|| philo->data->philos_count == 1)
	{
		pthread_mutex_unlock(*r_fork);
		return (0);
	}
	pthread_mutex_lock(*l_fork);
	if (!print(philo->data, philo->index, "has taken a fork")
		|| philo->data->philos_count == 1)
	{
		pthread_mutex_unlock(*r_fork);
		pthread_mutex_unlock(*l_fork);
		return (0);
	}
	return (1);
}

int	close_destroy(t_philo_info *philos_info)
{
	int	i;

	i = 0;
	while (i < philos_info->philos_count)
	{
		pthread_join(philos_info->philos_arr[i].thread_id, NULL);
		pthread_mutex_destroy(&(philos_info->forks_arr[i]));
		pthread_mutex_destroy(
			&(philos_info->philos_arr[i].after_last_meal_mutex));
		pthread_mutex_destroy(
			&(philos_info->philos_arr[i].count_he_ate_m));
		++i;
	}
	pthread_mutex_destroy(&(philos_info->finish_mutex));
	pthread_mutex_destroy(&(philos_info->print_mutex));
	free(philos_info->philos_arr);
	free(philos_info->forks_arr);
	free(philos_info);
	return (0);
}
