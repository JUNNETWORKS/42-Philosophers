#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "philosopher.h"
#include "utils.h"

static void	*thr_philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->mux);
	philo->status = THINKING;
	philo->last_eating_ms = get_current_time_ms();
	philo->eating_count = 0;
	pthread_mutex_unlock(&philo->mux);
	while (!is_philo_simulation_ended(philo->philos_info, philo))
	{
		if (philo->status == THINKING)
			philosopher_eat(philo->philos_info, philo);
		else if (philo->status == SLEEPING)
			philosopher_sleep(philo->philos_info, philo);
	}
	return ((void *)0);
}

int	start_philos(t_philos_info *philos_info, t_philo *philos)
{
	long	i;

	i = 0;
	while (i < philos_info->num_of_philos)
	{
		if (pthread_create(&philos[i].thread, NULL,
				thr_philosopher, (void *)(philos + i)))
		{
			printf("pthread_create() error!\n");
			return (-1);
		}
		pthread_detach(philos[i].thread);
		i += 2;
	}
	usleep(1000);
	i = 1;
	while (i < philos_info->num_of_philos)
	{
		if (pthread_create(&philos[i].thread, NULL,
				thr_philosopher, (void *)(philos + i)))
		{
			printf("pthread_create() error!\n");
			return (-1);
		}
		pthread_detach(philos[i].thread);
		i += 2;
	}
	return (0);
}

int	init_philos(t_philos_info *philos_info, t_philo *philos)
{
	long	i;

	i = 0;
	while (i < philos_info->num_of_philos)
	{
		philos[i].idx = i;
		philos[i].status = THINKING;
		philos[i].eating_count = 0;
		philos[i].last_eating_ms = get_current_time_ms();
		philos[i].is_living = true;
		philos[i].philos_info = philos_info;
		if (pthread_mutex_init(&philos[i].mux, NULL))
			return (1);
		i++;
	}
	return (0);
}
