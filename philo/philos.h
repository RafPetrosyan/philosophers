/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:23:50 by rafpetro          #+#    #+#             */
/*   Updated: 2024/08/01 18:02:00 by rafpetro         ###   ########.fr       */
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
	int				index;
	pthread_t 		thread_id;
	struct s_philos	*data;
	
} t_philo;


typedef struct s_philos
{
	int		philos_count;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		number_of_times_each_philosopher_must_eat;
	size_t	start_time;
	t_philo *arr;	
} t_philos;



int		ft_atoi(const char *nptr);
void	validacia(int argc, char **argv);

#endif