/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:24:17 by rafpetro          #+#    #+#             */
/*   Updated: 2024/09/30 15:57:59 by rafpetro         ###   ########.fr       */
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
	philo_info = malloc(sizeof(t_philo_info));
	if (philo_info == 0)
		return (printf("Memory allocation error!\n"));
	if (philo_info->count_eat == 0)
		return (0);
	init_philo_info(philo_info, argc, argv);
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
	pthread_mutex_lock(&(philo->number_of_times_he_ate_mutex));
	++philo->number_of_times_he_ate;
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
		pthread_mutex_destroy(&(philos_info->forks_arr[i]));
		pthread_mutex_destroy(
			&(philos_info->philos_arr[i].after_last_meal_mutex));
		pthread_mutex_destroy(
			&(philos_info->philos_arr[i].number_of_times_he_ate_mutex));
		i++;
	}
	pthread_mutex_destroy(&(philos_info->finish_mutex));
	pthread_mutex_destroy(&(philos_info->print_mutex));
	free(philos_info->philos_arr);
	free(philos_info->forks_arr);
	free(philos_info);
	printf("katareci");
	return (0);
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
		i++;
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
		pthread_mutex_lock(&(pinfo->philos_arr[i].number_of_times_he_ate_mutex));
		if (pinfo->philos_arr[i].number_of_times_he_ate
			>= pinfo->count_eat)
			++all_philos_get_finish_time;
		pthread_mutex_unlock(&(pinfo->philos_arr[i].number_of_times_he_ate_mutex));
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
