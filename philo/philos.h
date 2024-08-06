/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:23:50 by rafpetro          #+#    #+#             */
/*   Updated: 2024/08/06 16:42:07 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
# define PHILOS_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>

typedef struct s_philo
{
	int					index;
	pthread_t 			thread_id;
	int					number_of_times_he_ate;
	size_t				after_last_meal;
	struct s_philo_info	*data;
	
} t_philo;


typedef struct s_philo_info
{
	int				philos_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				count_eat;
	size_t			start_time;
	int				finish_time;
	t_philo			*philos_arr;
	pthread_mutex_t	*forks_arr;
	pthread_mutex_t	finish_mutex;
	pthread_mutex_t	print_mutex;
} t_philo_info;



int		ft_atoi(const char *nptr);
int		validacia(int argc, char **argv);
void	init_philos(t_philo_info **philo_info, int argc, char **argv);
void	init_mutex(t_philo_info *philos);

#endif