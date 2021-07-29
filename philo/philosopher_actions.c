#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

static void	get_fork_idxes(
	long num_of_forks, int *fork_idx, int *next_fork_idx, int philo_idx)
{
	/* 哲学者の人数によってどういう風にフォークを取るか変える手法 */
	if (num_of_forks % 2 == 0)
	{
		if (philo_idx % 2 == 1)
			usleep(200);
		*fork_idx = philo_idx;
		*next_fork_idx = (philo_idx + 1) % num_of_forks;
	}
	else
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
			// 哲学者が奇数の場合はスプーンのmutexのブロックを防ぐために200us待つ
			usleep(200);
			*fork_idx = (philo_idx + 1) % num_of_forks;
			*next_fork_idx = philo_idx % num_of_forks;
		}
	}


// 	/* 0番目の哲学者のみ i+1, i の順で取る手法.
// 	 * 0番目以外は i, i+1 の順で取る
// 	 */
// 	if (philo_idx == 0)
// 	{
// 		*fork_idx = 1;
// 		*next_fork_idx = 0;
// 	}
// 	else
// 	{
// 		*fork_idx = philo_idx;
// 		*next_fork_idx = (philo_idx + 1) % num_of_forks;
// 	}


// 	/* 哲学者の位置により右手か左手を優先する解法 */
// 	if (philo_idx % 2 == 0)
// 	{
// 		// 哲学者が偶数なら右のフォークを先に取る
// 		*fork_idx = philo_idx % num_of_forks;
// 		*next_fork_idx = (philo_idx + 1) % num_of_forks;
// 	}
// 	else
// 	{
// 		// 哲学者が奇数の場合はスプーンのmutexのブロックを防ぐために200us待つ
// 		usleep(20);
// 		*fork_idx = (philo_idx + 1) % num_of_forks;
// 		*next_fork_idx = philo_idx % num_of_forks;
// 	}

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
	hold_fork(philos_info->forks, fork_idx);
	if (is_philo_simulation_ended(philos_info, philo))
		return (release_forks_and_rtn_err(philos_info->forks, fork_idx, -1));
	write_philo_status(philo->idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	hold_fork(philos_info->forks, next_fork_idx);
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
	precise_sleep_ms(philos_info->time_to_eat_ms);
	philo->status = THINKING;
	if (is_philo_simulation_ended(philos_info, philo))
		return (1);
	write_philo_status(philo->idx, THINKING, get_current_time_ms());
	return (0);
}
