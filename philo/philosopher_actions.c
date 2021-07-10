#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

static void	get_fork_idxes(
	long num_of_forks, int *fork_idx, int *next_fork_idx, int philo_idx)
{
	if (philo_idx < num_of_forks - 1)
	{
		*fork_idx = philo_idx + 1;
		*next_fork_idx = philo_idx;
	}
	else
	{
		*fork_idx = num_of_forks - 1;
		*next_fork_idx = 0;
	}
}

static int	release_fork_and_rtn_err(pthread_mutex_t *forks, int fork_idx)
{
	release_fork(forks, fork_idx);
	return (-1);
}

/* リソース階層による解法
 * 各哲学者は大きいidxのフォークを最初に取る. その後小さいidxのフォークを取る風にする.
 * 食後は小さいフォークを解放してから大きいidxのフォークを解放する.
 */
int	philosopher_eat(t_philos_info *philos_info, t_philo *philo)
{
	int	fork_idx;
	int	next_fork_idx;

	get_fork_idxes(philo->philos_info->num_of_philos,
		&fork_idx, &next_fork_idx, philo->idx);
	hold_fork(philos_info->forks, fork_idx);
	if (!is_philo_still_alive(*philo->philos_info, *philo))
		return (release_fork_and_rtn_err(philos_info->forks, fork_idx));
	write_philo_status(philo->idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	hold_fork(philos_info->forks, next_fork_idx);
	if (!is_philo_still_alive(*philo->philos_info, *philo))
		return (release_fork_and_rtn_err(philos_info->forks, next_fork_idx));
	write_philo_status(philo->idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	if (!is_philo_still_alive(*philo->philos_info, *philo))
		return (1);
	write_philo_status(philo->idx, EATING, get_current_time_ms());
	philo->status = EATING;
	philo->last_eating_ms = get_current_time_ms();
	usleep(philos_info->time_to_eat_ms * 1000);
	release_fork(philos_info->forks, fork_idx);
	release_fork(philos_info->forks, next_fork_idx);
	philo->eating_count += 1;
	philo->status = SLEEPING;
	return (0);
}

int	philosopher_sleep(t_philos_info *philos_info, t_philo *philo)
{
	if (!is_philo_still_alive(*philo->philos_info, *philo))
		return (1);
	write_philo_status(philo->idx, SLEEPING, get_current_time_ms());
	usleep(philos_info->time_to_sleep_ms * 1000);
	philo->status = THINKING;
	if (!is_philo_still_alive(*philo->philos_info, *philo))
		return (1);
	write_philo_status(philo->idx, THINKING, get_current_time_ms());
	return (0);
}
