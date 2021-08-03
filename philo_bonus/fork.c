#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "philosopher.h"
#include "utils.h"

int	init_forks(pthread_mutex_t *forks, int fork_num)
{
	int	i;

	i = 0;
	while (i < fork_num)
	{
		if (pthread_mutex_init(&forks[i], NULL))
			return (-1);
		i++;
	}
	return (0);
}

void	hold_fork(sem_t *forks)
{
	sem_wait(forks);
}

void	release_fork(sem_t *forks)
{
	sem_post(forks);
}

void	release_forks(sem_t *forks, int num)
{
	while (num--)
		release_fork(forks);
}

int	release_forks_and_rtn_err(sem_t *forks, int num)
{
	release_forks(forks, num);
	return (-1);
}
