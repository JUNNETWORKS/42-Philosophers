#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "philosopher.h"
#include "utils.h"

bool	is_philo_still_alive(t_philos_info philos_info, t_philo philo)
{
	long	rest_time_ms;
	bool	has_eaten_n_times;

	rest_time_ms = philos_info.time_to_die_ms - (get_current_time_ms() - philo.last_eating_ms);
	has_eaten_n_times = false;
	if (philos_info.must_eat_times > 0)
		has_eaten_n_times = philo.eating_count >= philos_info.must_eat_times;
	return (rest_time_ms > 0 && !has_eaten_n_times);
}

int	start_philo_processes(t_philos_info philos_info, pid_t *philos_pid)
{
	long	i;
	pid_t	pid;

	i = 0;
	while (i < philos_info.num_of_philos)
	{
		pid = fork();
		if (pid == -1)
			return (-1);
		else if (pid == 0)
			philosopher_process(philos_info, i);
		philos_pid[i] = pid;
		i++;
	}
	return (0);
}

int	wait_philo_processes(t_philos_info philos_info, pid_t *philos_pid)
{
	long	i;

	i = 0;
	while (i < philos_info.num_of_philos)
	{
		waitpid(philos_pid[i], NULL, 0);
		i++;
	}
	return (0);
}
