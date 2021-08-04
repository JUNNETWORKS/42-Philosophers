#include <unistd.h>
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
	t_philo	*philo;
	bool	end_of_simulation;
	bool	has_eaten_n_times;
	long	rest_time_ms;

	philo = (t_philo *)arg;
	end_of_simulation = false;
	while (true)
	{
		usleep(100);
		pthread_mutex_lock(&philo->philos_info->mux);
		end_of_simulation = philo->philos_info->end_of_simulation;
		pthread_mutex_unlock(&philo->philos_info->mux);
		if (end_of_simulation)
			break;
		pthread_mutex_lock(&philo->mux);
		has_eaten_n_times
			= philo->eating_count >= philo->philos_info->must_eat_times;
		rest_time_ms = philo->philos_info->time_to_die_ms
			- (get_current_time_ms() - philo->last_eating_ms);
		if ((philo->philos_info->must_eat_times >= 0 && has_eaten_n_times)
			|| rest_time_ms <= 0)
		{
			philo->is_living = false;
			if (rest_time_ms <= 0)
			{
				pthread_mutex_lock(&philo->philos_info->mux);
				philo->philos_info->end_of_simulation = true;
				pthread_mutex_unlock(&philo->philos_info->mux);
				write_philo_status(philo->idx, DIED);
			}
			else
				write_philo_status(philo->idx, HAS_EATEN);
			pthread_mutex_unlock(&philo->mux);
			break ;
		}
		pthread_mutex_unlock(&philo->mux);
	}
	return ((void *)0);
}

int	start_philo_observers(t_philos_info *philos_info,
	t_philo *philos, pthread_t *philo_observers)
{
	long	i;

	i = 0;
	while (i < philos_info->num_of_philos)
	{
		if (pthread_create(&philo_observers[i], NULL,
				thr_philo_observer, (void *)(philos + i)))
		{
			printf("pthread_create() error!\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	wait_philo_observers(t_philos_info *philos_info, pthread_t *philo_observers)
{
	long	i;

	i = 0;
	while (i < philos_info->num_of_philos)
		pthread_join(philo_observers[i++], NULL);
	return (0);
}
