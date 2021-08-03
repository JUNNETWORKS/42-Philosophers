/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_find_idx.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 22:16:18 by jtanaka           #+#    #+#             */
/*   Updated: 2021/08/03 22:16:18 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <wait.h>
#include "utils.h"

int	arr_find_pid(pid_t *arr, pid_t value, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (arr[i] == value)
			return (i);
		i++;
	}
	return (-1);
}
