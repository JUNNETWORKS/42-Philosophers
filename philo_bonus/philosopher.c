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

#include <stdio.h>
int		wait_philos(t_philos_info *philos_info, pid_t *philo_pids)
{
	pid_t	died_pid;
	int		exit_status;
	int		philo_num;

	philo_num = philos_info->num_of_philos;
	while (philo_num--)
	{
		died_pid = waitpid(0, &exit_status, 0);
		printf("died_pid: %d\n", died_pid);
		if (WIFEXITED(exit_status))
		{
			exit_status = WEXITSTATUS(exit_status);
		printf("philo %d exit_status: %d\n",
			arr_find_pid(philo_pids, died_pid, philos_info->num_of_philos) + 1,
			exit_status);
		}
		philo_pids[arr_find_pid(philo_pids, died_pid, philos_info->num_of_philos)] = -1;
		if (exit_status == PHILO_END_DIE || exit_status == PHILO_END_ERR)
			break ;
	}
	philo_num = 0;
	while (philo_num < philos_info->num_of_philos)
	{
		if (philo_pids[philo_num] != -1)
		{
			printf("kill philo %d\n", philo_num + 1);
			kill(philo_pids[philo_num], SIGKILL);
		}
		philo_num++;
	}
	return (0);
}
