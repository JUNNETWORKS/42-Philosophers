#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philospher.h"

t_philos_info	g_philos_info;
pthread_mutex_t	*g_folks;
pthread_t		*g_philos;

void	*thr_philosopher(void *arg)
{
	long	philo_no = (long)arg;
	printf("This is Philo %ld\n", philo_no);
	return ((void *)0);
}

int	main(int argc, char **argv)
{
	if (parse_philos_argv(argc, argv))
		return (1);

	g_folks = malloc(sizeof(pthread_mutex_t) * g_philos_info.num_of_philos);
	g_philos = malloc(sizeof(pthread_t) * g_philos_info.num_of_philos);
	if (!g_folks || !g_philos)
		return (1);

	long	i = 0;
	while (i < g_philos_info.num_of_philos)
	{
		if (pthread_create(&g_philos[i], NULL, thr_philosopher, (void *)i))
		{
			printf("pthread_create() error!\n");
			return (1);
		}
		i++;
	}

	i = 0;
	while ( i < g_philos_info.num_of_philos)
	{
		pthread_join(g_philos[i], NULL);
		i++;
	}

	printf("Hello Philosophers!!\n");
	return (0);
}
