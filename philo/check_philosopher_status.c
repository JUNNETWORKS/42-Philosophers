#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "philosopher.h"
#include "utils.h"

bool	is_philo_simulation_ended(t_philos_info *philos_info, t_philo *philo)
{
	bool	end_of_simulation;
	bool	is_philo_living;

	pthread_mutex_lock(&philos_info->mux);
	end_of_simulation = philos_info->end_of_simulation;
	pthread_mutex_unlock(&philos_info->mux);
	pthread_mutex_lock(&philo->mux);
	is_philo_living = philo->is_living;
	pthread_mutex_unlock(&philo->mux);
	return (end_of_simulation || !is_philo_living);
}

bool	has_philo_eaten_n_times(t_philo *philo)
{
	bool	has_eaten_n_times;

	if (philo->philos_info->must_eat_times < 0)
		return (false);
	pthread_mutex_lock(&philo->mux);
	has_eaten_n_times = philo->eating_count >= philo->philos_info->must_eat_times;
	pthread_mutex_unlock(&philo->mux);
	return (has_eaten_n_times);
}

bool	is_philo_still_alive(t_philo *philo)
{
	long	rest_time_ms;

	pthread_mutex_lock(&philo->mux);
	rest_time_ms = philo->philos_info->time_to_die_ms
		- (get_current_time_ms() - philo->last_eating_ms);
	pthread_mutex_unlock(&philo->mux);
	return (rest_time_ms > 0);
}

