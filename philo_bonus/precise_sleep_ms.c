/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precise_sleep_ms.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 22:16:20 by jtanaka           #+#    #+#             */
/*   Updated: 2021/08/03 22:16:20 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "utils.h"

void	precise_sleep_ms(long ms)
{
	long	current_time_ms;

	current_time_ms = get_current_time_ms();
	while (get_current_time_ms() - current_time_ms < ms)
		usleep(100);
}
