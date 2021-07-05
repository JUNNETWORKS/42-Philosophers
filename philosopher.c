#include <stdio.h>
#include <pthread.h>
#include "philosopher.h"
#include "utils.h"

bool	is_philo_still_alive(t_philo *philo, long time_to_die_ms, long must_eat_times)
{
	long	rest_time_ms;
	bool	has_eaten_n_times;

	rest_time_ms = time_to_die_ms - (get_current_time_ms() - philo->last_eating_ms);
	has_eaten_n_times = false;
	if (must_eat_times > 0)
		has_eaten_n_times = philo->eating_count >= must_eat_times;
	return (rest_time_ms > 0 || !has_eaten_n_times);
}

