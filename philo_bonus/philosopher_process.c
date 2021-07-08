#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "philosopher.h"
#include "utils.h"

void	philosopher_process(t_philos_info philos_info, long philo_idx)
{
	t_philo	philo;

	philo.idx = philo_idx;
	philo.status = THINKING;
	philo.last_eating_ms = get_current_time_ms();
	philo.eating_count = 0;
	philo.is_living = true;
	if (is_philo_still_alive(philos_info, philo))
		write_philo_status(philo_idx, THINKING, get_current_time_ms());
	// TODO: 死活監視スレッドの作成
	while (is_philo_still_alive(philos_info, philo))
	{
		if (philo.status == THINKING)
			philosopher_eat(philos_info, &philo);
		else if (philo.status == SLEEPING)
			philosopher_sleep(philos_info, &philo);
	}
	exit(0);
}
