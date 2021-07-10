#include <stdio.h>
#include <pthread.h>
#include "philosopher.h"
#include "utils.h"

static void	*thr_philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->status = THINKING;
	philo->last_eating_ms = get_current_time_ms();
	philo->eating_count = 0;
	if (is_philo_still_alive(*philo->philos_info, *philo))
		write_philo_status(philo->idx, THINKING, get_current_time_ms());
	while (is_philo_still_alive(*philo->philos_info, *philo))
	{
		if (philo->status == THINKING)
			philosopher_eat(philo->philos_info, philo);
		else if (philo->status == SLEEPING)
			philosopher_sleep(philo->philos_info, philo);
	}
	return ((void *)0);
}

bool	is_philo_still_alive(t_philos_info philos_info, t_philo philo)
{
	long	rest_time_ms;
	bool	has_eaten_n_times;

	rest_time_ms = philos_info.time_to_die_ms
		- (get_current_time_ms() - philo.last_eating_ms);
	has_eaten_n_times = false;
	if (philos_info.must_eat_times > 0)
		has_eaten_n_times
			= philo.eating_count >= philos_info.must_eat_times;
	return (rest_time_ms > 0 && !has_eaten_n_times);
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
		i++;
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
		philos[i].last_eating_ms = get_current_time_ms();
		philos[i].eating_count = 0;
		philos[i].is_living = true;
		philos[i].philos_info = philos_info;
		i++;
	}
	return (0);
}
