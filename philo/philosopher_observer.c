#include <unistd.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

int	start_philo_observers(t_philos_info *philos_info,
	t_philo *philos, pthread_t *philo_observers)
{
	long	i;

	usleep(1000);
	i = 0;
	while (i < philos_info->num_of_philos)
	{
		if (pthread_create(&philo_observers[i], NULL,
				thr_philo_observer, (void *)(philos + i)))
		{
			printf("pthread_create() error!\n");
			return (-1);
		}
		pthread_detach(philo_observers[i]);
		i++;
	}
	return (0);
}

static bool	has_all_philos_eaten(t_philos_info *philos_info, t_philo *philos)
{
	int		i;
	bool	has_all_philos_eaten;

	i = 0;
	has_all_philos_eaten = true;
	while (i < philos_info->num_of_philos && has_all_philos_eaten)
	{
		pthread_mutex_lock(&philos[i].mux);
		if (philos[i].eating_count < philos_info->must_eat_times)
			has_all_philos_eaten = false;
		pthread_mutex_unlock(&philos[i].mux);
		i++;
	}
	return (has_all_philos_eaten);
}

int	wait_philo_observers(t_philos_info *philos_info, t_philo *philos)
{
	bool	end_of_simulation;

	end_of_simulation = false;
	while (!end_of_simulation)
	{
		if (philos_info->must_eat_times >= 0)
		{
			if (has_all_philos_eaten(philos_info, philos))
				break ;
		}
		pthread_mutex_lock(&philos_info->mux);
		end_of_simulation = philos_info->end_of_simulation;
		pthread_mutex_unlock(&philos_info->mux);
		usleep(200);
	}
	usleep(1000);
	return (0);
}
