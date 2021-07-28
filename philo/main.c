#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "philosopher.h"
#include "utils.h"

static void	wait_philos(t_philos_info *philos_info, t_philo *philos)
{
	int	i;
	int	will_stop_simulation;

	i = 0;
	will_stop_simulation = false;
	while (!will_stop_simulation)
	{
		if (!is_philo_still_alive(philos_info, &philos[i]))
		{
			write_philo_status(i, DIED, get_current_time_ms());
			will_stop_simulation = true;
		}
		else if (has_philo_eaten_n_times(philos_info, &philos[i]))
			will_stop_simulation = true;
		i = (i + 1) % philos_info->num_of_philos;
	}
}

int	main(int argc, char **argv)
{
	pthread_t		*philo_observers;
	t_philos_info	philos_info;
	t_philo			*philos;

	if (parse_philos_argv(argc, argv, &philos_info))
		return (1);
	philos = malloc(sizeof(t_philo) * philos_info.num_of_philos);
	philos_info.forks = malloc(
			sizeof(pthread_mutex_t) * philos_info.num_of_philos);
	philo_observers = malloc(
			sizeof(pthread_mutex_t) * philos_info.num_of_philos);
	if (!philos || !philos_info.forks || !philo_observers
		|| init_forks(philos_info.forks, philos_info.num_of_philos)
		|| init_philos(&philos_info, philos)
		|| start_philos(&philos_info, philos))
		// || start_philo_observers(&philos_info, philos, philo_observers))
		return (1);
	// wait_philo_observers(&philos_info, philo_observers);
	wait_philos(&philos_info, philos);
	return (0);
}
