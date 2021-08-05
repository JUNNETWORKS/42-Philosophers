#include <unistd.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

static bool	has_philo_simulation_ended(
				t_philos_info *philos_info);
static void	end_simulation(
				t_philos_info *philos_info, t_philo *philo);

/* 哲学者の死活監視スレッド
 *
 * 哲学者が死亡した時, 10msの誤差内でメッセージを出す必要があるので
 * 各哲学者に付き1死活監視スレッドが必要になる
 */
void	*thr_philo_observer(void *arg)
{
	t_philo			*philo;
	t_philos_info	*philos_info;
	long			rest_time_ms;

	philo = (t_philo *)arg;
	philos_info = philo->philos_info;
	while (true)
	{
		usleep(200);
		if (has_philo_simulation_ended(philos_info))
			break ;
		pthread_mutex_lock(&philo->mux);
		rest_time_ms = philos_info->time_to_die_ms
			- (get_current_time_ms() - philo->last_eating_ms);
		pthread_mutex_unlock(&philo->mux);
		if (rest_time_ms <= 0)
		{
			end_simulation(philos_info, philo);
			break ;
		}
	}
	return ((void *)0);
}

static bool	has_philo_simulation_ended(
	t_philos_info *philos_info)
{
	bool	end_of_simulation;

	pthread_mutex_lock(&philos_info->mux);
	end_of_simulation = philos_info->end_of_simulation;
	pthread_mutex_unlock(&philos_info->mux);
	return (end_of_simulation);
}

static void	end_simulation(t_philos_info *philos_info, t_philo *philo)
{
	pthread_mutex_lock(&philos_info->mux);
	philos_info->end_of_simulation = true;
	write_philo_status(&philos_info->writing_mux, philo->idx, DIED);
	pthread_mutex_unlock(&philos_info->mux);
}
