#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

int	init_forks(pthread_mutex_t *forks, int fork_num)
{
	int	i;

	i = 0;
	while (i < fork_num)
	{
		if (pthread_mutex_init(forks + i, NULL))
			return (-1);
		i++;
	}
	return (0);
}

void	hold_fork(pthread_mutex_t *forks, int fork_idx)
{
	pthread_mutex_lock(&forks[fork_idx]);
}

void	release_fork(pthread_mutex_t *forks, int fork_idx)
{
	pthread_mutex_unlock(&forks[fork_idx]);
}
