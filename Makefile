NAME := philo
CC := gcc
CFLAGS := -Werror -Wall -Wextra -g
# flags -MMD -MP -MF for generating dependency files
CFLAGS += -MMD -MP -MF

SRCS := main.c
OBJS := ${SRCS:.c=.o}
DEPS := ${SRCS:.c=.d}

all: ${NAME}

%.o: %.c
	$(CC) $(CFLAGS) $(<:.c=.d) -c $< -o $@

-include $(DEPS)
$(NAME): $(OBJS)
	$(LIBFT_MAKE)
	$(CC) -o $(NAME) $(OBJS) -lbsd

clean:
	${RM} ${OBJS}
	${RM} ${DEPS}

fclean: clean
	${RM} ${NAME}

re: fclean all

bonus: $(NAME)

.PHONY: all clean fclean re bonus
