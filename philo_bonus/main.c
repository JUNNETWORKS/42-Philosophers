#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "philosopher.h"
#include "utils.h"

int	main(int argc, char **argv)
{
	t_philos_info	philos_info;
	sem_t			*forks;
	sem_t			*has_died;

	if (parse_philos_argv(argc, argv, &philos_info))
		return (1);
	sem_unlink(SEM_FORKS_STR);
	sem_unlink(SEM_HAS_DIED_STR);
	forks = sem_open(SEM_FORKS_STR, O_CREAT | O_EXCL, S_IRWXU, philos_info.num_of_philos);
	has_died = sem_open(SEM_HAS_DIED_STR, O_CREAT | O_EXCL, S_IRWXU, 0);
	philos_info.philo_pids = malloc(sizeof(pid_t) * philos_info.num_of_philos);
	if (!forks || !has_died || !philos_info.philo_pids)
		return (1);
	printf("start philosopher bonus!!\n");
	start_philos(&philos_info, philos_info.philo_pids);
	wait_philos(&philos_info, philos_info.philo_pids);
	sem_unlink(SEM_FORKS_STR);
	return (0);
}
