#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

int	philosopher_eat(int philo_idx)
{
	int	fork_idx;
	int	next_fork_idx;

// 	/* 哲学者の位置により右手か左手を優先する解法 */
// 	if (philo_idx % 2 == 0)
// 	{
// 		// 哲学者が偶数なら右のフォークを先に取る
// 		fork_idx = philo_idx % g_philos_info.num_of_philos;
// 		next_fork_idx = (philo_idx + 1) % g_philos_info.num_of_philos;
// 	}
// 	else
// 	{
// 		// 哲学者が奇数の場合はスプーンのmutexのブロックを防ぐために10us待つ
// 		usleep(10);
// 		fork_idx = (philo_idx + 1) % g_philos_info.num_of_philos;
// 		next_fork_idx = philo_idx % g_philos_info.num_of_philos;
// 	}

	/* リソース階層による解法
	 * 各哲学者は大きいidxのフォークを最初に取る. その後小さいidxのフォークを取る風にする.
	 * 食後は小さいフォークを解放してから大きいidxのフォークを解放する.
	 */
	if (philo_idx < g_philos_info.num_of_philos - 1)
	{
		fork_idx = philo_idx + 1;
		next_fork_idx = philo_idx;
	}
	else
	{
		fork_idx = g_philos_info.num_of_philos - 1;
		next_fork_idx = 0;
	}

	hold_fork(fork_idx);
	if (!g_philos[philo_idx].is_living)
	{
		release_fork(fork_idx);
		return (1);
	}
	write_philo_status(philo_idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	hold_fork(next_fork_idx);
	if (!g_philos[philo_idx].is_living)
	{
		release_fork(next_fork_idx);
		return (1);
	}
	write_philo_status(philo_idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	write_philo_status(philo_idx, EATING, get_current_time_ms());
	g_philos[philo_idx].status = EATING;
	g_philos[philo_idx].last_eating_ms = get_current_time_ms();
	usleep(g_philos_info.time_to_eat_ms * 1000);
	release_fork(fork_idx);
	release_fork(next_fork_idx);
	g_philos[philo_idx].eating_count += 1;
	g_philos[philo_idx].status = SLEEPING;
	return (0);
}

int	philosopher_sleep(int philo_idx)
{
	if (!g_philos[philo_idx].is_living)
		return (1);
	write_philo_status(philo_idx, SLEEPING, get_current_time_ms());
	usleep(g_philos_info.time_to_sleep_ms * 1000);
	g_philos[philo_idx].status = THINKING;
	if (!g_philos[philo_idx].is_living)
		return (1);
	write_philo_status(philo_idx, THINKING, get_current_time_ms());
	return (0);
}
