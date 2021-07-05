#include <stdio.h>
#include <pthread.h>
#include "philosopher.h"

int	init_g_philos(void)
{
	long	i;

	i = 0;
	while (i < g_philos_info.num_of_philos)
	{
		g_philos->is_living = true;
		if (pthread_create(&g_philos[i].thread, NULL, thr_philosopher, (void *)i))
		{
			printf("pthread_create() error!\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	monitor_if_philosophers_are_living(void)
{
	long	i;
	int	philo_has_died_count;  // 死んだ哲学者の数

	i = 0;
	philo_has_died_count = 0;
	while (philo_has_died_count < g_philos_info.num_of_philos)
	{
		// 各哲学者の死活管理を行う
		// 死んでることが確認出来たら pthread_detach() でスレッドを終了させる
		if (g_philos[i].is_living && !is_philo_still_alive(&g_philos[i],
				g_philos_info.time_to_die_ms, g_philos_info.must_eat_times))
		{
			pthread_detach(g_philos[i].thread);
			g_philos[i].is_living = false;
			write_philo_status(i, DIED);
			philo_has_died_count++;
		}
		i++;
		i = i % g_philos_info.num_of_philos;
	}
	return (0);
}
