/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validacia.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafpetro <rafpetro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:48:50 by rafpetro          #+#    #+#             */
/*   Updated: 2024/09/30 15:48:58 by rafpetro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	validacia(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) == -1)
		{
			printf("Invalid Argument !\n");
			return (1);
		}
		if (i != argc - 1 && ft_atoi(argv[i]) == 0)
		{
			printf("Invalid Argument !\n");
			return (1);
		}
		++i;
	}
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int		i;
	size_t	numb;

	i = 0;
	numb = 0;
	if (!(nptr[i] >= '0' && nptr[i] <= '9'))
		return (-1);
	if (nptr[i] == '0' && nptr[i + 1] != '\0')
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		numb = numb * 10 + nptr[i] - '0';
		++i;
	}
	if (nptr[i] != '\0')
		return (-1);
	if (i >= 11 || numb > 2147483647)
		return (-1);
	return (numb);
}
