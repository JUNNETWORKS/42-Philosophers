#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "philospher.h"
#include "utils.h"

t_philos_info	g_philos_info;
t_philo			*g_philos;
pthread_mutex_t	*g_forks;
pthread_t		*g_philo_thrs;
pthread_mutex_t	g_stdout_mutex;

bool	is_philo_still_alive(int philo_idx)
{
	long	rest_time_ms;
	bool	has_eaten_n_times;

	rest_time_ms = g_philos_info.time_to_die_ms - (get_current_time_ms() - g_philos[philo_idx].last_eating_ms);
	has_eaten_n_times = false;
	if (g_philos_info.must_eat_times > 0)
		has_eaten_n_times = g_philos[philo_idx].eating_count >= g_philos_info.must_eat_times;
	return (rest_time_ms > 0 || !has_eaten_n_times);
}

void	*thr_philosopher(void *arg)
{
	long	philo_idx = (long)arg;

	g_philos[philo_idx].status = THINKING;
	g_philos[philo_idx].last_eating_ms = get_current_time_ms();
	g_philos[philo_idx].eating_count = 0;
	while (is_philo_still_alive(philo_idx))
	{
		if (g_philos[philo_idx].status == THINKING)
			philospher_eat(philo_idx);
		else if (g_philos[philo_idx].status == SLEEPING)
			philospher_sleep(philo_idx);
	}
	write_philo_status(philo_idx, DIED);
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
	return (0);
}
