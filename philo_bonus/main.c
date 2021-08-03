/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 22:16:19 by jtanaka           #+#    #+#             */
/*   Updated: 2021/08/03 22:16:19 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "philosopher.h"
#include "utils.h"

int	main(int argc, char **argv)
{
	t_philos_info	philos_info;
	sem_t			*forks;
	sem_t			*has_died;

	if (parse_philos_argv(argc, argv, &philos_info))
		return (1);
	sem_unlink(SEM_FORKS_STR);
	sem_unlink(SEM_HAS_DIED_STR);
	forks = sem_open(SEM_FORKS_STR, O_CREAT | O_EXCL,
			S_IRWXU, philos_info.num_of_philos);
	has_died = sem_open(SEM_HAS_DIED_STR, O_CREAT | O_EXCL, S_IRWXU, 0);
	philos_info.philo_pids = malloc(sizeof(pid_t) * philos_info.num_of_philos);
	if (!forks || !has_died || !philos_info.philo_pids)
		return (1);
	start_philos(&philos_info);
	wait_philos(&philos_info);
	sem_unlink(SEM_FORKS_STR);
	return (0);
}
