#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "philosopher.h"
#include "utils.h"

void	*thr_philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->sem);
	philo->status = THINKING;
	philo->last_eating_ms = get_current_time_ms();
	philo->eating_count = 0;
	sem_post(philo->sem);
	while (!is_philo_simulation_ended(philo->philos_info, philo))
	{
		if (philo->status == THINKING)
			philosopher_eat(philo->philos_info, philo);
		else if (philo->status == SLEEPING)
			philosopher_sleep(philo->philos_info, philo);
	}
	return ((void *)0);
}

static int	start_philo(t_philo *philo)
{
	if (pthread_create(&philo->thread, NULL,
			thr_philosopher, (void *)(philo)))
	{
		printf("pthread_create() error!\n");
		return (-1);
	}
	pthread_detach(philo->thread);
	return (0);
}

static char	*get_sem_str(int idx)
{
	char	*idx_str;
	char	*sem_str;

	idx_str = ft_itoa(idx);
	if (!idx_str)
		return (NULL);
	sem_str = ft_strjoin("/philo_", idx_str);
	if (!sem_str)
		return (free_and_rtn_null(idx_str));
	free(idx_str);
	return (sem_str);
}

static t_philo	*generate_philo(t_philos_info *philos_info, int idx)
{
	t_philo		*philo;
	char		*sem_str;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		exit(PHILO_END_ERR);
	philo->idx = idx;
	philo->eating_count = 0;
	philo->status = THINKING;
	philo->is_living = true;
	philo->philos_info = philos_info;
	philo->last_eating_ms = get_current_time_ms();
	sem_str = get_sem_str(idx);
	if (!sem_str)
		return (free_and_rtn_null(philo));
	sem_unlink(sem_str);
	philo->sem = sem_open(sem_str, O_CREAT | O_EXCL, S_IRWXU, 1);
	free(sem_str);
	if (philo->sem == SEM_FAILED)
		return (free_and_rtn_null(philo));
	return (philo);
}

void	philo_process(t_philos_info *philos_info, int idx)
{
	t_philo		*philo;

	philo = generate_philo(philos_info, idx);
	philos_info->forks = sem_open(SEM_FORKS_STR, O_EXCL, S_IRWXU);
	if (!philo || !philos_info->forks
		|| start_philo(philo))
		exit(PHILO_END_ERR);
	usleep(100);
	exit(observe_philo(philos_info, philo));
}
