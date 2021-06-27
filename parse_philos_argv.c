#include <stdio.h>
#include "philospher.h"
#include "utils.h"

int	parse_philos_argv(int argc, char **argv)
{
	int	error;

	if (argc < 5)
	{
		printf("arguments must be\n");
		printf("{program_name} number_of_philosophers time_to_die time_to_eat "
			"time_to_sleep [number_of_times_each_philosopher_must_eat]");
		return (1);
	}
	error = 0;
	error |= ft_atoi(argv[1], &g_philos_info.num_of_philos);
	error |= ft_atoi(argv[2], &g_philos_info.time_to_die_ms);
	error |= ft_atoi(argv[3], &g_philos_info.time_to_eat_ms);
	error |= ft_atoi(argv[4], &g_philos_info.time_to_sleep_ms);
	if (argc == 6)
		error |= ft_atoi(argv[5], &g_philos_info.must_eat_times);
	else
		g_philos_info.must_eat_times = -1;
	if (error)
	{
		printf("argv is invalid!\n");
		return (-1);
	}
	return (0);
}
