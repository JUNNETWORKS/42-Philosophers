/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 22:03:01 by jtanaka           #+#    #+#             */
/*   Updated: 2021/08/03 22:03:01 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

int	init_forks(pthread_mutex_t *forks, int fork_num)
{
	int	i;

	i = 0;
	while (i < fork_num)
	{
		if (pthread_mutex_init(&forks[i], NULL))
			return (-1);
		i++;
	}
	return (0);
}

void	hold_fork(pthread_mutex_t *forks, int fork_idx)
{
	pthread_mutex_lock(&forks[fork_idx]);
}

void	release_fork(pthread_mutex_t *forks, int fork_idx)
{
	pthread_mutex_unlock(&forks[fork_idx]);
}

void	release_forks(pthread_mutex_t *forks, int fork_idx1, int fork_idx2)
{
	if (fork_idx1 >= 0)
		release_fork(forks, fork_idx1);
	if (fork_idx2 >= 0)
		release_fork(forks, fork_idx2);
}

int	release_forks_and_rtn_err(pthread_mutex_t *forks,
	int fork_idx1, int fork_idx2)
{
	release_forks(forks, fork_idx1, fork_idx2);
	return (-1);
}
