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

	philo = (t_philo *)arg;
	while (!is_philo_simulation_ended(philo->philos_info, philo))
	{
		if (!is_philo_still_alive(philo))
		{
			pthread_mutex_lock(&philo->mux);
			pthread_mutex_lock(&philo->philos_info->mux);
			philo->is_living = false;
			philo->philos_info->end_of_simulation = true;
			pthread_mutex_unlock(&philo->philos_info->mux);
			pthread_mutex_unlock(&philo->mux);
			write_philo_status(philo->idx, DIED, get_current_time_ms());
			break ;
		}
		if (has_philo_eaten_n_times(philo))
		{
			pthread_mutex_lock(&philo->mux);
			philo->is_living = false;
			pthread_mutex_unlock(&philo->mux);
			write_philo_status(philo->idx, HAS_EATEN, get_current_time_ms());
			break ;
		}
		usleep(100);
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
