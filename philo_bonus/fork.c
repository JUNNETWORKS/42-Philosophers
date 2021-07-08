#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

int	init_g_forks(int fork_num)
{
	return (0);
}

void	hold_fork(int fork_idx)
{
	pthread_mutex_lock(&g_forks[fork_idx]);
}

void	release_fork(int fork_idx)
{
	pthread_mutex_unlock(&g_forks[fork_idx]);
}
