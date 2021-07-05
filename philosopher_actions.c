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

	/* 哲学者の位置により右手か左手を優先する解法 */
	if (philo_idx % 2 == 0)
	{
		// 哲学者が偶数なら右のフォークを先に取る
		fork_idx = philo_idx % g_philos_info.num_of_philos;
		next_fork_idx = (philo_idx + 1) % g_philos_info.num_of_philos;
	}
	else
	{
		// 哲学者が奇数の場合はスプーンのmutexのブロックを防ぐために10ms待つ
		usleep(10);
		fork_idx = (philo_idx + 1) % g_philos_info.num_of_philos;
		next_fork_idx = philo_idx % g_philos_info.num_of_philos;
	}

	hold_fork(fork_idx);
	write_philo_status(philo_idx, HAS_TAKEN_A_FORK);
	hold_fork(next_fork_idx);
	write_philo_status(philo_idx, HAS_TAKEN_A_FORK);
	write_philo_status(philo_idx, EATING);
	g_philos[philo_idx].status = EATING;
	usleep(g_philos_info.time_to_eat_ms);
	release_fork(next_fork_idx);
	release_fork(fork_idx);
	g_philos[philo_idx].last_eating_ms = get_current_time_ms();
	g_philos[philo_idx].eating_count += 1;
	g_philos[philo_idx].status = SLEEPING;
	return (0);
}

int	philosopher_sleep(int philo_idx)
{
	write_philo_status(philo_idx, SLEEPING);
	usleep(g_philos_info.time_to_sleep_ms);
	g_philos[philo_idx].status = THINKING;
	write_philo_status(philo_idx, THINKING);
	return (0);
}
