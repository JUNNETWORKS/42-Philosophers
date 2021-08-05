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
	t_philo			*philo;
	t_philos_info	*philos_info;
	bool			end_of_simulation;
	long			rest_time_ms;

	philo = (t_philo *)arg;
	philos_info = philo->philos_info;
	end_of_simulation = false;
	while (true)
	{
		usleep(200);
		pthread_mutex_lock(&philos_info->mux);
		end_of_simulation = philos_info->end_of_simulation;
		pthread_mutex_unlock(&philos_info->mux);
		if (end_of_simulation)
			break;
		pthread_mutex_lock(&philo->mux);
		rest_time_ms = philos_info->time_to_die_ms
			- (get_current_time_ms() - philo->last_eating_ms);
		pthread_mutex_unlock(&philo->mux);
		if (rest_time_ms <= 0)
		{
			pthread_mutex_lock(&philos_info->mux);
			philos_info->end_of_simulation = true;
			pthread_mutex_unlock(&philos_info->mux);
			write_philo_status(&philos_info->writing_mux, philo->idx, DIED);
			break ;
		}
	}
	return ((void *)0);
}

int	start_philo_observers(t_philos_info *philos_info,
	t_philo *philos, pthread_t *philo_observers)
{
	long	i;

	usleep(1000);
	i = 0;
	while (i < philos_info->num_of_philos)
	{
		if (pthread_create(&philo_observers[i], NULL,
				thr_philo_observer, (void *)(philos + i)))
		{
			printf("pthread_create() error!\n");
			return (-1);
		}
		pthread_detach(philo_observers[i]);
		i++;
	}
	return (0);
}

int	wait_philo_observers(t_philos_info *philos_info, t_philo *philos)
{
	bool	end_of_simulation;
	bool	has_all_philos_eaten;
	int		i;

	end_of_simulation = false;
	while (!end_of_simulation)
	{
		// check if all philosophers has eaten completely
		if (philos_info->must_eat_times >= 0)
		{
			i = 0;
			has_all_philos_eaten = true;
			while (i < philos_info->num_of_philos && has_all_philos_eaten)
			{
				pthread_mutex_lock(&philos[i].mux);
				if (philos[i].eating_count < philos_info->must_eat_times)
					has_all_philos_eaten = false;
				pthread_mutex_unlock(&philos[i].mux);
				i++;
			}
			if (has_all_philos_eaten)
				break;
		}
		pthread_mutex_lock(&philos_info->mux);
		end_of_simulation = philos_info->end_of_simulation;
		pthread_mutex_unlock(&philos_info->mux);
		usleep(200);
	}
	usleep(1000);
	return (0);
}
