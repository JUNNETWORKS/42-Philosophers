#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "philosopher.h"
#include "utils.h"

int	main(int argc, char **argv)
{
	pthread_t		*philo_observers;
	t_philos_info	*philos_info;
	t_philo			*philos;

	philos_info = malloc(sizeof(t_philos_info));
	if (!philos_info || parse_philos_argv(argc, argv, philos_info))
		return (1);
	philos = malloc(sizeof(t_philo) * philos_info->num_of_philos);
	philos_info->forks = malloc(
			sizeof(pthread_mutex_t) * philos_info->num_of_philos);
	philos_info->end_of_simulation = false;
	philo_observers = malloc(
			sizeof(pthread_t) * philos_info->num_of_philos);
	if (!philos || !philos_info->forks || !philo_observers
		|| pthread_mutex_init(&philos_info->mux, NULL)
		|| pthread_mutex_init(&philos_info->writing_mux, NULL)
		|| init_forks(philos_info->forks, philos_info->num_of_philos)
		|| init_philos(philos_info, philos)
		|| start_philos(philos_info, philos)
		|| start_philo_observers(philos_info, philos, philo_observers))
		return (1);
	wait_philo_observers(philos_info, philo_observers);
	return (0);
}
