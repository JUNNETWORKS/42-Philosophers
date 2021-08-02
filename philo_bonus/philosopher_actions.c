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
	write_philo_status(philo->idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	hold_fork(philos_info->forks);
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, 2));
	write_philo_status(philo->idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, 2));
	write_philo_status(philo->idx, EATING, get_current_time_ms());
	return (0);
}

/* リソース階層による解法
 * 各哲学者は大きいidxのフォークを最初に取る. その後小さいidxのフォークを取る風にする.
 * 食後は小さいフォークを解放してから大きいidxのフォークを解放する.
 */
int	philosopher_eat(t_philos_info *philos_info, t_philo *philo)
{
	if (philo_try_2_hold_forks(philos_info, philo))
		return (-1);
	pthread_mutex_lock(&philo->mux);
	philo->status = EATING;
	philo->last_eating_ms = get_current_time_ms();
	pthread_mutex_unlock(&philo->mux);
	precise_sleep_ms(philos_info->time_to_eat_ms);
	release_forks(philos_info->forks, 2);
	pthread_mutex_lock(&philo->mux);
	philo->eating_count += 1;
	philo->status = SLEEPING;
	pthread_mutex_unlock(&philo->mux);
	return (0);
}

int	philosopher_sleep(t_philos_info *philos_info, t_philo *philo)
{
	if (is_philo_simulation_ended(philos_info, philo))
		return (1);
	write_philo_status(philo->idx, SLEEPING, get_current_time_ms());
	precise_sleep_ms(philos_info->time_to_sleep_ms);
	philo->status = THINKING;
	if (is_philo_simulation_ended(philos_info, philo))
		return (1);
	write_philo_status(philo->idx, THINKING, get_current_time_ms());
	return (0);
}
