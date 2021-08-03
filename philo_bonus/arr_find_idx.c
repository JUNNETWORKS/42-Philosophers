#include <stdbool.h>
#include <sys/wait.h>
#include "utils.h"

long	arr_find_pid(pid_t *arr, pid_t value, long len)
{
	long	i;

	i = 0;
	while (i < len)
	{
		if (arr[i] == value)
			return (i);
		i++;
	}
	return (-1);
}
