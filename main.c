#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philospher.h"
#include "utils.h"

t_philos_info	g_philos_info;
t_philo			*g_philos;
pthread_mutex_t	*g_forks;
pthread_t		*g_philo_thrs;

void	*thr_philosopher(void *arg)
{
	long	philo_no = (long)arg;

	g_philos[philo_no].status = THINKING;
	g_philos[philo_no].last_eating_ms = get_current_time_ms();
	while (get_current_time_ms() - g_philos[philo_no].last_eating_ms < g_philos_info.time_to_die_ms)
	{
		if (g_philos[philo_no].status == EATING)
			philospher_eat(philo_no);
		else if (g_philos[philo_no].status == SLEEPING)
			philospher_sleep(philo_no);
		else if (g_philos[philo_no].status == THINKING)
			philospher_think(philo_no);
	}
	printf("This is Philo %ld %ld[ms]\n", philo_no, g_philos[philo_no].last_eating_ms);
	return ((void *)0);
}

int	main(int argc, char **argv)
{
	long	i;

	if (parse_philos_argv(argc, argv))
		return (1);
	g_philos = malloc(sizeof(t_philo) * g_philos_info.num_of_philos);
	g_forks = malloc(sizeof(pthread_mutex_t) * g_philos_info.num_of_philos);
	g_philo_thrs = malloc(sizeof(pthread_t) * g_philos_info.num_of_philos);
	if (!g_philos || !g_forks || !g_philo_thrs
		|| init_g_forks(g_philos_info.num_of_philos))
		return (1);
	i = 0;
	while (i < g_philos_info.num_of_philos)
	{
		if (pthread_create(&g_philo_thrs[i], NULL, thr_philosopher, (void *)i))
		{
			printf("pthread_create() error!\n");
			return (1);
		}
		i++;
	}
	i = 0;
	while ( i < g_philos_info.num_of_philos)
	{
		pthread_join(g_philo_thrs[i], NULL);
		i++;
	}
	printf("Hello Philosophers!!\n");
	return (0);
}
