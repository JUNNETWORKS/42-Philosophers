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
	pthread_mutex_lock(&philo->mux);
	philo->status = THINKING;
	philo->last_eating_ms = get_current_time_ms();
	philo->eating_count = 0;
	pthread_mutex_unlock(&philo->mux);
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

void	philo_process(t_philos_info *philos_info, int idx)
{
	t_philo		*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		exit(PHILO_END_ERR);
	philo->idx = idx;
	philo->eating_count = 0;
	philo->status = THINKING;
	philo->is_living = true;
	philo->philos_info = philos_info;
	philo->last_eating_ms = get_current_time_ms();
	philos_info->forks = sem_open(SEM_FORKS_STR, O_EXCL, S_IRWXU);
	if (!philos_info->forks || pthread_mutex_init(&philo->mux, NULL)
		|| start_philo(philo))
		exit(PHILO_END_ERR);
	usleep(100);
	exit(observe_philo(philos_info, philo));
}
