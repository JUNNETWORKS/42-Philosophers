#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

static void	get_fork_idxes(int *fork_idx, int *next_fork_idx, int philo_idx)
{
	if (philo_idx < g_philos_info.num_of_philos - 1)
	{
		*fork_idx = philo_idx + 1;
		*next_fork_idx = philo_idx;
	}
	else
	{
		*fork_idx = g_philos_info.num_of_philos - 1;
		*next_fork_idx = 0;
	}
}

static int	release_fork_and_rtn_err(int fork_idx)
{
	release_fork(fork_idx);
	return (-1);
}

/* リソース階層による解法
 * 各哲学者は大きいidxのフォークを最初に取る. その後小さいidxのフォークを取る風にする.
 * 食後は小さいフォークを解放してから大きいidxのフォークを解放する.
 */
int	philosopher_eat(int philo_idx)
{
	int	fork_idx;
	int	next_fork_idx;

	get_fork_idxes(&fork_idx, &next_fork_idx, philo_idx);
	hold_fork(fork_idx);
	if (!is_philo_still_alive(philo_idx))
		return (release_fork_and_rtn_err(fork_idx));
	write_philo_status(philo_idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	hold_fork(next_fork_idx);
	if (!is_philo_still_alive(philo_idx))
		return (release_fork_and_rtn_err(next_fork_idx));
	write_philo_status(philo_idx, HAS_TAKEN_A_FORK, get_current_time_ms());
	if (!is_philo_still_alive(philo_idx))
		return (1);
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
	if (!is_philo_still_alive(philo_idx))
		return (1);
	write_philo_status(philo_idx, SLEEPING, get_current_time_ms());
	usleep(g_philos_info.time_to_sleep_ms * 1000);
	g_philos[philo_idx].status = THINKING;
	if (!is_philo_still_alive(philo_idx))
		return (1);
	write_philo_status(philo_idx, THINKING, get_current_time_ms());
	return (0);
}
