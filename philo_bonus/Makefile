NAME := philo_bonus
CC := gcc
CFLAGS := -Werror -Wall -Wextra
# CFLAGS += -g -fsanitize=thread
# flags -MMD -MP -MF for generating dependency files
CFLAGS += -MMD -MP
LINK_LIB := -pthread
ifeq ($(shell uname), Linux)
        LINK_LIB += -lbsd
endif
LDFLAGS := $(LINK_LIB)

OBJS_DIR := objs
DEPS_DIR := deps

SRCS := main.c \
  philosopher.c philosopher_actions.c parse_philos_argv.c philosopher_writer.c \
  philosopher_process.c philosopher_observer.c check_philosopher_status.c  wait_philosophers.c \
  fork.c \
  get_current_time_ms.c precise_sleep_ms.c arr_find_idx.c free_utils.c \
  ft_atol.c ft_isdigit.c ft_strncmp.c ft_strjoin.c ft_strlen.c ft_ltoa.c ft_calloc.c
OBJS := $(addprefix $(OBJS_DIR)/, ${SRCS:.c=.o})
DEPS := $(addprefix $(DEPS_DIR)/, ${SRCS:.c=.d})

all: ${NAME}

$(OBJS_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -MF $(DEPS_DIR)/$(<:.c=.d) -c $< -o $@

-include $(DEPS)
$(NAME): $(OBJS)
	$(LIBFT_MAKE)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	${RM} ${OBJS}
	${RM} ${DEPS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
