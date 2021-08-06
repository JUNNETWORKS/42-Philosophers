#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "philosopher.h"
#include "utils.h"

static int	philo_try_2_hold_forks(t_philos_info *philos_info, t_philo *philo)
{
	hold_fork(philos_info->forks);
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, 1));
	write_philo_status(philo->philos_info->sem_writing,
		philo->idx, HAS_TAKEN_A_FORK);
	hold_fork(philos_info->forks);
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, 2));
	write_philo_status(philo->philos_info->sem_writing,
		philo->idx, HAS_TAKEN_A_FORK);
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, 2));
	write_philo_status(philo->philos_info->sem_writing,
		philo->idx, EATING);
	return (0);
}

int	philosopher_eat(t_philos_info *philos_info, t_philo *philo)
{
	if (philo_try_2_hold_forks(philos_info, philo))
		return (-1);
	sem_wait(philo->sem);
	philo->status = EATING;
	philo->last_eating_ms = get_current_time_ms();
	sem_post(philo->sem);
	precise_sleep_ms(philos_info->time_to_eat_ms);
	release_forks(philos_info->forks, 2);
	sem_wait(philo->sem);
	philo->eating_count += 1;
	philo->status = SLEEPING;
	sem_post(philo->sem);
	return (0);
}

int	philosopher_sleep(t_philos_info *philos_info, t_philo *philo)
{
	if (is_philo_simulation_ended(philos_info, philo))
		return (1);
	write_philo_status(philo->philos_info->sem_writing, philo->idx, SLEEPING);
	precise_sleep_ms(philos_info->time_to_sleep_ms);
	philo->status = THINKING;
	if (is_philo_simulation_ended(philos_info, philo))
		return (1);
	write_philo_status(philo->philos_info->sem_writing, philo->idx, THINKING);
	return (0);
}
