#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

/* 哲学者の人数によってどういう風にフォークを取るか変える手法 */
static void	get_fork_idxes(
	long num_of_forks, int *fork_idx, int *next_fork_idx, int philo_idx)
{
	if (num_of_forks % 2 == 0)
	{
		if (philo_idx % 2 == 1)
			usleep(200);
		*fork_idx = philo_idx;
		*next_fork_idx = (philo_idx + 1) % num_of_forks;
	}
	else
	{
		if (philo_idx % 2 == 0)
		{
			usleep(500);
		}
		*fork_idx = philo_idx;
		*next_fork_idx = (philo_idx + 1) % num_of_forks;
	}
}

static int	philo_try_2_hold_forks(t_philos_info *philos_info, t_philo *philo,
	int fork_idx, int next_fork_idx)
{
	hold_fork(philos_info->forks, fork_idx);
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, fork_idx, -1));
	write_philo_status(philo->idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	hold_fork(philos_info->forks, next_fork_idx);
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks,
				next_fork_idx, fork_idx));
	write_philo_status(philo->idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks,
				next_fork_idx, fork_idx));
	write_philo_status(philo->idx, EATING, get_current_time_ms());
	return (0);
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
	if (philo_try_2_hold_forks(philos_info, philo, fork_idx, next_fork_idx))
		return (-1);
	pthread_mutex_lock(&philo->mux);
	philo->status = EATING;
	philo->last_eating_ms = get_current_time_ms();
	pthread_mutex_unlock(&philo->mux);
	precise_sleep_ms(philos_info->time_to_eat_ms);
	release_forks(philos_info->forks, next_fork_idx, fork_idx);
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
