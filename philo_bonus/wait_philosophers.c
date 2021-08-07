#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "philosopher.h"
#include "utils.h"

static void	*thr_observe_has_philo_died(void *arg)
{
	sem_t			*has_died;
	t_philos_info	*philos_info;
	long			i;

	philos_info = (t_philos_info *)arg;
	has_died = sem_open(SEM_HAS_DIED_STR, O_EXCL, S_IRWXU);
	if (!has_died)
		return ((void *)1);
	sem_wait(has_died);
	i = 0;
	while (i < philos_info->num_of_philos)
		kill(philos_info->philo_pids[i++], SIGTERM);
	sem_close(has_died);
	return (0);
}

static void	*thr_observe_has_philo_eaten_completely(void *arg)
{
	sem_t			*philo_has_eaten_count;
	t_philos_info	*philos_info;
	long			i;

	philos_info = (t_philos_info *)arg;
	philo_has_eaten_count = sem_open(SEM_HAS_EATEN_COUNT_STR, O_EXCL, S_IRWXU);
	if (!philo_has_eaten_count)
		return ((void *)1);
	i = 0;
	while (i++ < philos_info->num_of_philos)
		sem_wait(philo_has_eaten_count);
	sem_close(philo_has_eaten_count);
	i = 0;
	while (i < philos_info->num_of_philos)
		kill(philos_info->philo_pids[i++], SIGTERM);
	return (0);
}

static int	start_philos_observer(t_philos_info *philos_info)
{
	pthread_t	thr_id;

	if (pthread_create(&thr_id, NULL,
			thr_observe_has_philo_died, (void *)philos_info))
		return (-1);
	pthread_detach(thr_id);
	if (pthread_create(&thr_id, NULL,
			thr_observe_has_philo_eaten_completely, (void *)philos_info))
		return (-1);
	pthread_detach(thr_id);
	return (0);
}

int	wait_philos(t_philos_info *philos_info)
{
	int		exit_status;
	long	philo_num;

	if (start_philos_observer(philos_info))
		return (-1);
	philo_num = philos_info->num_of_philos;
	while (philo_num--)
		waitpid(0, &exit_status, 0);
	return (0);
}
