CC = cc

CFLAGS = -Wall -Wextra -Werror -I ./

NAME = philo

SRCS = main.c source.c validacia.c inits.c

OBJS = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(OBJS) Makefile philos.h
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean 
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re
