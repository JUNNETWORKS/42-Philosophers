#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

static void	get_fork_idxes(
	long num_of_forks, long *fork_idx, long *next_fork_idx, long philo_idx)
{
	*fork_idx = philo_idx;
	*next_fork_idx = (philo_idx + 1) % num_of_forks;
}

static int	philo_try_2_hold_forks(t_philos_info *philos_info, t_philo *philo,
	long fork_idx, long next_fork_idx)
{
	hold_fork(philos_info->forks, fork_idx);
	if (is_philo_simulation_ended(philos_info))
		return (release_forks_and_rtn_err(philos_info->forks, fork_idx, -1));
	write_philo_status(&philos_info->writing_mux, philo->idx, HAS_TAKEN_A_FORK);
	hold_fork(philos_info->forks, next_fork_idx);
	if (is_philo_simulation_ended(philos_info))
		return (release_forks_and_rtn_err(philos_info->forks,
				next_fork_idx, fork_idx));
	write_philo_status(&philos_info->writing_mux, philo->idx, HAS_TAKEN_A_FORK);
	if (is_philo_simulation_ended(philos_info))
		return (release_forks_and_rtn_err(philos_info->forks,
				next_fork_idx, fork_idx));
	write_philo_status(&philos_info->writing_mux, philo->idx, EATING);
	return (0);
}

/* リソース階層による解法
 * 各哲学者は大きいidxのフォークを最初に取る. その後小さいidxのフォークを取る風にする.
 * 食後は小さいフォークを解放してから大きいidxのフォークを解放する.
 */
int	philosopher_eat(t_philos_info *philos_info, t_philo *philo)
{
	long	fork_idx;
	long	next_fork_idx;

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
	if (is_philo_simulation_ended(philos_info))
		return (1);
	write_philo_status(&philos_info->writing_mux, philo->idx, SLEEPING);
	precise_sleep_ms(philos_info->time_to_sleep_ms);
	philo->status = THINKING;
	if (is_philo_simulation_ended(philos_info))
		return (1);
	write_philo_status(&philos_info->writing_mux, philo->idx, THINKING);
	return (0);
}
