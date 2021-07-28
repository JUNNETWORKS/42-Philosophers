#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

static void	get_fork_idxes(
	long num_of_forks, int *fork_idx, int *next_fork_idx, int philo_idx)
{
	/* 哲学者の位置により右手か左手を優先する解法 */
	if (philo_idx % 2 == 0)
	{
		// 哲学者が偶数なら右のフォークを先に取る
		*fork_idx = philo_idx % num_of_forks;
		*next_fork_idx = (philo_idx + 1) % num_of_forks;
	}
	else
	{
		// 哲学者が奇数の場合はスプーンのmutexのブロックを防ぐために10us待つ
		usleep(10);
		*fork_idx = (philo_idx + 1) % num_of_forks;
		*next_fork_idx = philo_idx % num_of_forks;
	}

// 	/* リソース階層による解法
// 	 * 各哲学者は大きいidxのフォークを最初に取る. その後小さいidxのフォークを取る風にする.
// 	 * 食後は小さいフォークを解放してから大きいidxのフォークを解放する.
// 	 */
// 	if (philo_idx < num_of_forks - 1)
// 	{
// 		*fork_idx = philo_idx + 1;
// 		*next_fork_idx = philo_idx;
// 	}
// 	else
// 	{
// 		*fork_idx = 0;
// 		*next_fork_idx = num_of_forks - 1;
// 	}
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
	// printf("start hold_fork (%ld): %d\n", philo->idx, fork_idx);
	hold_fork(philos_info->forks, fork_idx);
	// printf("end hold_fork (%ld): %d\n", philo->idx, fork_idx);
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, fork_idx, -1));
	write_philo_status(philo->idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	// printf("start hold_fork (%ld): %d\n", philo->idx, next_fork_idx);
	hold_fork(philos_info->forks, next_fork_idx);
	// printf("end hold_fork (%ld): %d\n", philo->idx, next_fork_idx);
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, next_fork_idx, fork_idx));
	write_philo_status(philo->idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, next_fork_idx, fork_idx));
	write_philo_status(philo->idx, EATING, get_current_time_ms());
	pthread_mutex_lock(&philo->mux);
	philo->status = EATING;
	philo->last_eating_ms = get_current_time_ms();
	pthread_mutex_unlock(&philo->mux);
	usleep(philos_info->time_to_eat_ms * 1000);
	pthread_mutex_lock(&philo->mux);
	// release_forks(philos_info->forks, fork_idx, next_fork_idx);
	printf("start release_fork (%ld): %d\n", philo->idx, next_fork_idx);
	release_fork(philos_info->forks, next_fork_idx);
	// printf("end release_fork (%ld): %d\n", philo->idx, next_fork_idx);
	printf("start release_fork (%ld): %d\n", philo->idx, fork_idx);
	release_fork(philos_info->forks, fork_idx);
	// printf("end release_fork (%ld): %d\n", philo->idx, fork_idx);
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
	usleep(philos_info->time_to_sleep_ms * 1000);
	pthread_mutex_lock(&philo->mux);
	philo->status = THINKING;
	pthread_mutex_unlock(&philo->mux);
	if (is_philo_simulation_ended(philos_info, philo))
		return (1);
	write_philo_status(philo->idx, THINKING, get_current_time_ms());
	return (0);
}
