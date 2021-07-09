#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

int	philosopher_eat(t_philos_info philos_info, t_philo *philo)
{
	long	philo_idx;
	sem_t	*forks;

	forks = sem_open(SEM_STR, O_CREAT | O_EXCL, S_IRWXU, philos_info.num_of_philos);
	philo_idx = philo->idx;
	// TODO: セマフォを使った解法
	philo->status = EATING;
	philo->last_eating_ms = get_current_time_ms();
	usleep(philos_info.time_to_eat_ms * 1000);
	philo->eating_count += 1;
	philo->status = SLEEPING;
	return (0);
}

int	philosopher_sleep(t_philos_info philos_info, t_philo *philo)
{
	long	philo_idx;

	philo_idx = philo->idx;
	if (!is_philo_still_alive(philos_info, *philo))
		return (1);
	write_philo_status(philo_idx, SLEEPING, get_current_time_ms());
	usleep(philos_info.time_to_sleep_ms * 1000);
	philo->status = THINKING;
	if (!is_philo_still_alive(philos_info, *philo))
		return (1);
	write_philo_status(philo_idx, THINKING, get_current_time_ms());
	return (0);
}
