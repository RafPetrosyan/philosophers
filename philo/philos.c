#include "philos.h"

int	main(int argc, char **argv)
{
	if (!(argc == 5 || argc == 6))
		exit(write(2, "Invalid Argument !\n", 19));
	validacia(argc, argv);
}