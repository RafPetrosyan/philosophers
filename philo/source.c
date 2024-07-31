#include "philos.h"

void	validacia(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) == -1)
			exit(write(2, "Invalid Argument !\n", 19));
		if (i != argc - 1 && ft_atoi(argv[i]) == 0)
			exit(write(2, "Invalid Argument !\n", 19));
		++i;
	}
}