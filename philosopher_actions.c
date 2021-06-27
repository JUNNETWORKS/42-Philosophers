#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philospher.h"
#include "utils.h"

int	philospher_eat(int philo_idx)
{
	int	next_fork_idx;
	int	prev_fork_idx;

	next_fork_idx = (philo_idx + 1) % g_philos_info.num_of_philos;
	if (philo_idx == 0)
		prev_fork_idx = g_philos_info.num_of_philos - 1;
	prev_fork_idx = (philo_idx - 1) % g_philos_info.num_of_philos;

	hold_fork(next_fork_idx);
	hold_fork(prev_fork_idx);
	usleep(g_philos_info.time_to_eat_ms);
	release_fork(next_fork_idx);
	release_fork(prev_fork_idx);
	g_philos[philo_idx].status = SLEEPING;
	return (0);
}

int	philospher_sleep(int philo_idx)
{
	usleep(g_philos_info.time_to_sleep_ms);
	g_philos[philo_idx].status = THINKING;
	return (0);
}

int	philospher_think(int philo_idx)
{
	usleep(g_philos_info.time_to_sleep_ms);
	g_philos[philo_idx].status = EATING;
	return (0);
}
