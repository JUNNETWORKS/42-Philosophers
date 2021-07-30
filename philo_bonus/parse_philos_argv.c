#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

static int	print_err_msg_and_rtn(void)
{
	printf("arguments must be\n");
	printf("{program_name} number_of_philosophers time_to_die time_to_eat "
		"time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	return (-1);
}

int	parse_philos_argv(int argc, char **argv, t_philos_info *philos_info)
{
	int	error;

	if (argc < 5)
		return (print_err_msg_and_rtn());
	error = 0;
	error |= ft_atol(argv[1], &philos_info->num_of_philos);
	error |= ft_atol(argv[2], &philos_info->time_to_die_ms);
	error |= ft_atol(argv[3], &philos_info->time_to_eat_ms);
	error |= ft_atol(argv[4], &philos_info->time_to_sleep_ms);
	if (argc == 6)
	{
		error |= ft_atol(argv[5], &philos_info->must_eat_times);
		error |= philos_info->must_eat_times < 0;
	}
	else
		philos_info->must_eat_times = -1;
	if (error || philos_info->num_of_philos < 0
		|| philos_info->time_to_die_ms < 0
		|| philos_info->time_to_eat_ms < 0
		|| philos_info->time_to_sleep_ms < 0)
	{
		printf("argv is invalid!\n");
		return (-1);
	}
	return (0);
}
