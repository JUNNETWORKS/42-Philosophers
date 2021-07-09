#include <unistd.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

int	start_philo_observers(pthread_t	*philo_observers)
{
	long	i;

	i = 0;
	while (i < philos_info.num_of_philos)
	{

		if (pthread_create(&philo_observers[i], NULL, thr_philo_observer, (void *)i))
		{
			printf("pthread_create() error!\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	wait_philo_observers(pthread_t	*philo_observers)
{
	long	i;

	i = 0;
	while (i < philos_info.num_of_philos)
		pthread_join(philo_observers[i++], NULL);
	return (0);
}

