/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:23:50 by rafpetro          #+#    #+#             */
/*   Updated: 2024/09/30 15:54:56 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
# define PHILOS_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>

typedef struct s_philo
{
	int					index;
	pthread_t			thread_id;
	int					number_of_times_he_ate;
	size_t				after_last_meal;
	pthread_mutex_t		after_last_meal_mutex;
	pthread_mutex_t		number_of_times_he_ate_mutex;
	struct s_philo_info	*data;
}	t_philo;

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
}	t_philo_info;

int			ft_atoi(const char *nptr);
int			validacia(int argc, char **argv);
int			error_handler(int i, t_philo_info *philo_info);
void		init_philo_info(t_philo_info *philo_info, int argc, char **argv);
void		create_threads(t_philo_info *pinfo);
void		*routine(void *philo_void);
void		init_mutex(t_philo_info *philo_info);
int			eating(t_philo *philo, pthread_mutex_t *left_fork,
				pthread_mutex_t *right_fork);
long long	get_time(void);
int			get_finish_time(t_philo_info *philos);
int			close_destroy(t_philo_info *philos_info);
int			print(t_philo_info *philo_info, int index, char *message);
int			taking_forks(t_philo *philo,
				pthread_mutex_t **l_fork, pthread_mutex_t **r_fork);
int			check_dead(t_philo_info *philos);
int			check_eaten(t_philo_info *philos);
void		ft_usleep(long long mls, t_philo_info *philosophers);
#endif