#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "philosopher.h"
#include "utils.h"

static t_philo	g_philo;

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
		if (!is_philo_still_alive(g_philos_info, g_philo))
		{
			write_philo_status(philo_idx, DIED, get_current_time_ms());
			return ((void *)0);
		}
	}
	return ((void *)0);
}

void	philosopher_process(t_philos_info philos_info, long philo_idx)
{
	pthread_t	observer_thr_id;

	// 初期化
	g_philo.idx = philo_idx;
	g_philo.status = THINKING;
	g_philo.last_eating_ms = get_current_time_ms();
	g_philo.eating_count = 0;
	// 死活監視スレッドの作成
	if (pthread_create(&observer_thr_id, NULL, thr_philo_observer, (void *)philo_idx))
		exit(1);
	if (is_philo_still_alive(philos_info, g_philo))
		write_philo_status(philo_idx, THINKING, get_current_time_ms());
	while (is_philo_still_alive(philos_info, g_philo))
	{
		if (g_philo.status == THINKING)
			philosopher_eat(philos_info, &g_philo);
		else if (g_philo.status == SLEEPING)
			philosopher_sleep(philos_info, &g_philo);
	}
	pthread_join(observer_thr_id, NULL);
	exit(0);
}
