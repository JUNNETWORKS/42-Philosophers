/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_writer.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 22:03:01 by jtanaka           #+#    #+#             */
/*   Updated: 2021/08/03 22:03:01 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

/* {timestamp_in_ms} {philo_no} {status_msg} */
void	write_philo_status(int philo_idx, enum e_philo_status status, long ms)
{
	int		philo_no;

	philo_no = philo_idx + 1;
	if (status == HAS_TAKEN_A_FORK)
		printf("%ld %d has taken a fork\n", ms, philo_no);
	else if (status == EATING)
		printf("%ld %d is eating\n", ms, philo_no);
	else if (status == SLEEPING)
		printf("%ld %d is sleeping\n", ms, philo_no);
	else if (status == THINKING)
		printf("%ld %d is thinking\n", ms, philo_no);
	else if (status == DIED)
		printf("%ld %d died\n", ms, philo_no);
	else if (status == HAS_EATEN)
		printf("%ld %d has eaten completely\n", ms, philo_no);
}
