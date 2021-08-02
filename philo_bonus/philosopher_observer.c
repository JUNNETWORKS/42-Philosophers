#include <unistd.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

static void	set_philos_simulation_end(t_philo *philo)
{
	pthread_mutex_lock(&philo->mux);
	philo->is_living = false;
	pthread_mutex_unlock(&philo->mux);
	write_philo_status(philo->idx, DIED, get_current_time_ms());
}

static void	set_philo_has_eaten_completely(t_philo *philo)
{
	pthread_mutex_lock(&philo->mux);
	philo->is_living = false;
	pthread_mutex_unlock(&philo->mux);
	write_philo_status(philo->idx, HAS_EATEN, get_current_time_ms());
}

/* 哲学者の死活監視
 */
int	observe_philo(t_philos_info *philos_info, t_philo *philo)
{
	(void)philos_info;
	while (true)
	{
		if (!is_philo_still_alive(philo))
		{
			set_philos_simulation_end(philo);
			return (PHILO_END_DIE);
		}
		usleep(100);
		if (has_philo_eaten_n_times(philo))
		{
			set_philo_has_eaten_completely(philo);
			return (PHILO_END_EATEN);
		}
		usleep(100);
	}
}

