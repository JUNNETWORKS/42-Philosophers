#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include "philosopher.h"
#include "utils.h"

bool	is_philo_simulation_ended(t_philos_info *philos_info, t_philo *philo)
{
	bool	is_philo_living;

	(void)philos_info;
	sem_wait(philo->sem);
	is_philo_living = philo->is_living;
	sem_post(philo->sem);
	return (!is_philo_living);
}

bool	has_philo_eaten_n_times(t_philo *philo)
{
	bool	has_eaten_n_times;

	if (philo->philos_info->must_eat_times < 0)
		return (false);
	sem_wait(philo->sem);
	has_eaten_n_times
		= philo->eating_count >= philo->philos_info->must_eat_times;
	sem_post(philo->sem);
	return (has_eaten_n_times);
}

bool	is_philo_still_alive(t_philo *philo)
{
	long	rest_time_ms;

	sem_wait(philo->sem);
	rest_time_ms = philo->philos_info->time_to_die_ms
		- (get_current_time_ms() - philo->last_eating_ms);
	sem_post(philo->sem);
	return (rest_time_ms > 0);
}
