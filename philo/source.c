/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:24:22 by rafpetro          #+#    #+#             */
/*   Updated: 2024/09/30 15:53:52 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

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
	return (0);
}
