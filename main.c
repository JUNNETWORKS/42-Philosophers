#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "philosopher.h"
#include "utils.h"

t_philos_info	g_philos_info;
t_philo			*g_philos;
pthread_mutex_t	*g_forks;

int	main(int argc, char **argv)
{
	long		i;

	if (parse_philos_argv(argc, argv))
		return (1);
	g_philos = malloc(sizeof(t_philo) * g_philos_info.num_of_philos);
	g_forks = malloc(sizeof(pthread_mutex_t) * g_philos_info.num_of_philos);
	if (!g_philos || !g_forks
		|| init_g_forks(g_philos_info.num_of_philos) || start_g_philos())
		return (1);
	i = 0;
	while (i < g_philos_info.num_of_philos)
		pthread_join(g_philos[i++].thread, NULL);
	return (0);
}
