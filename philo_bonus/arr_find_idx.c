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
			return i;
		i++;
	}
	return -1;
}
