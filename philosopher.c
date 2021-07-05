#include <stdio.h>
#include <pthread.h>
#include "philosopher.h"
#include "utils.h"

static void	*thr_philosopher(void *arg)
{
	long	philo_idx;

	philo_idx = (long)arg;
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

bool	is_philo_still_alive(t_philo *philo, long time_to_die_ms, long must_eat_times)
{
	long	rest_time_ms;
	bool	has_eaten_n_times;

	rest_time_ms = time_to_die_ms - (get_current_time_ms() - philo->last_eating_ms);
	has_eaten_n_times = false;
	if (must_eat_times > 0)
		has_eaten_n_times = philo->eating_count >= must_eat_times;
	return (rest_time_ms > 0 || !has_eaten_n_times);
}

int	start_g_philos(void)
{
	long	i;

	i = 0;
	while (i < g_philos_info.num_of_philos)
	{
		g_philos[i].is_living = true;
		if (pthread_create(&g_philos[i].thread, NULL, thr_philosopher, (void *)i))
		{
			printf("pthread_create() error!\n");
			return (-1);
		}
		i++;
	}
	return (0);
}
