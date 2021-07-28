NAME := philo
CC := gcc
CFLAGS := -Werror -Wall -Wextra -g
# flags -MMD -MP -MF for generating dependency files
CFLAGS += -MMD -MP
LINK_LIB := -pthread
LINK_LIB += -lbsd
LDFLAGS := $(LINK_LIB)

OBJS_DIR := objs
DEPS_DIR := deps

SRCS := main.c \
  philosopher.c philosopher_actions.c parse_philos_argv.c philosopher_writer.c \
  philosopher_observer.c \
  fork.c ft_atol.c ft_isdigit.c ft_strncmp.c get_current_time_ms.c
OBJS := $(addprefix $(OBJS_DIR)/, ${SRCS:.c=.o})
DEPS := $(addprefix $(DEPS_DIR)/, ${SRCS:.c=.d})

all: ${NAME}

$(OBJS_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -MF $(DEPS_DIR)/$(<:.c=.d) -c $< -o $@

-include $(DEPS)
$(NAME): $(OBJS)
	$(LIBFT_MAKE)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	${RM} ${OBJS}
	${RM} ${DEPS}

fclean: clean
	${RM} ${NAME}

re: fclean all

bonus: $(NAME)

.PHONY: all clean fclean re bonus
