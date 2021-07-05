#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "philosopher.h"
#include "utils.h"

t_philos_info	g_philos_info;
t_philo			*g_philos;
pthread_mutex_t	*g_forks;

void	*thr_philosopher(void *arg)
{
	long	philo_idx = (long)arg;

	g_philos[philo_idx].status = THINKING;
	g_philos[philo_idx].last_eating_ms = get_current_time_ms();
	g_philos[philo_idx].eating_count = 0;
	while (1)
	{
		if (g_philos[philo_idx].status == THINKING)
			philosopher_eat(philo_idx);
		else if (g_philos[philo_idx].status == SLEEPING)
			philosopher_sleep(philo_idx);
	}
	return ((void *)0);
}

int	main(int argc, char **argv)
{
	if (parse_philos_argv(argc, argv))
		return (1);
	g_philos = malloc(sizeof(t_philo) * g_philos_info.num_of_philos);
	g_forks = malloc(sizeof(pthread_mutex_t) * g_philos_info.num_of_philos);
	if (!g_philos || !g_forks || init_g_forks(g_philos_info.num_of_philos)
		|| init_g_philos())
		return (1);
	monitor_if_philosophers_are_living();
	return (0);
}
