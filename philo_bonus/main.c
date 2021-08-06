#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "philosopher.h"
#include "utils.h"

static void	unlink_semaphores(long philo_num);

int	main(int argc, char **argv)
{
	t_philos_info	philos_info;
	sem_t			*forks;

	if (parse_philos_argv(argc, argv, &philos_info))
		return (1);
	unlink_semaphores(philos_info.num_of_philos);
	forks = sem_open(SEM_FORKS_STR, O_CREAT | O_EXCL,
			S_IRWXU, philos_info.num_of_philos);
	philos_info.sem_writing = sem_open(SEM_WRITING_STR,
			O_CREAT | O_EXCL, S_IRWXU, 1);
	philos_info.philo_pids = malloc(sizeof(pid_t) * philos_info.num_of_philos);
	if (!forks || !philos_info.philo_pids
		|| !sem_open(SEM_HAS_DIED_STR, O_CREAT | O_EXCL, S_IRWXU, 0)
		|| !philos_info.sem_writing)
		return (1);
	start_philos(&philos_info);
	wait_philos(&philos_info);
	unlink_semaphores(philos_info.num_of_philos);
	return (0);
}

static void	unlink_semaphores(long philo_num)
{
	long	i;
	char	*philo_sem;

	sem_unlink(SEM_FORKS_STR);
	sem_unlink(SEM_HAS_DIED_STR);
	sem_unlink(SEM_WRITING_STR);
	i = 0;
	while (i < philo_num)
	{
		philo_sem = get_philo_sem_str(i);
		if (!philo_sem)
			continue ;
		sem_unlink(philo_sem);
		free(philo_sem);
		i++;
	}
}
