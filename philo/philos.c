/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:24:17 by rafpetro          #+#    #+#             */
/*   Updated: 2024/08/01 18:05:12 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void *myThreadFunction(void *arg)
{
    ++(*(int*)arg);
    return NULL;
}

int	main(int argc, char **argv)
{
	t_philos	*philos;
	if (!(argc == 5 || argc == 6))
		exit(write(2, "Invalid number of arguments\n", 28));
	validacia(argc, argv);
	philos = (t_philos *)malloc(sizeof(t_philos));
	if (philos == 0)
		return (1);
	philos->philos_count = ft_atoi(argv[1]);
	philos->time_to_die = ft_atoi(argv[2]);
	philos->time_to_die = ft_atoi(argv[3]);
	philos->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		philos->number_of_times_each_philosopher_must_eat = ft_atoi(argv[1]);
	else
		philos->number_of_times_each_philosopher_must_eat = -1;
	int i = 0;
	//malloc
	while (i < philos->philos_count)
	{
		// philos->arr[i]
	}
}
