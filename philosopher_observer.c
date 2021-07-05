#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

/* 哲学者の死活監視スレッド
 *
 * 哲学者が死亡した時, 10msの誤差内でメッセージを出す必要があるので
 * 各哲学者に付き1死活監視スレッドが必要になる
 */
static void	*thr_philo_observer(void *arg)
{
	long	philo_idx;

	philo_idx = (long)arg;
	while (1)
	{
		if (!is_philo_still_alive(&g_philos[philo_idx],
			g_philos_info.time_to_die_ms, g_philos_info.must_eat_times))
		{
			write_philo_status(philo_idx, DIED, get_current_time_ms());
			g_philos[philo_idx].is_living = false;
			return ((void *)0);
		}
	}
	return ((void *)0);
}

int	start_philo_observers(pthread_t	*philo_observers)
{
	long	i;

	i = 0;
	while (i < g_philos_info.num_of_philos)
	{

		if (pthread_create(&philo_observers[i], NULL, thr_philo_observer, (void *)i))
		{
			printf("pthread_create() error!\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	wait_philo_observers(pthread_t	*philo_observers)
{
	long	i;

	i = 0;
	while (i < g_philos_info.num_of_philos)
		pthread_join(philo_observers[i++], NULL);
	return (0);
}

