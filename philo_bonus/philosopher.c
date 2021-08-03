#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "philosopher.h"
#include "utils.h"

static int	start_philo_process(t_philos_info *philos_info, pid_t *philo_pids, int idx)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	else if (pid == 0)
		philo_process(philos_info, idx);
	philo_pids[idx] = pid;
	return (0);
}

int		start_philos(t_philos_info *philos_info, pid_t *philo_pids)
{
	int		i;

	i = 0;
	while (i < philos_info->num_of_philos)
	{
		if (start_philo_process(philos_info, philo_pids, i))
			return (-1);
		i += 2;
	}
	usleep(500);
	i = 1;
	while (i < philos_info->num_of_philos)
	{
		if (start_philo_process(philos_info, philo_pids, i))
			return (-1);
		i += 2;
	}
	return (0);
}
