#include "utils.h"

int	ltoa_recursive(char *result, long n)
{
	long	idx;
	long	sign;

	idx = 0;
	sign = 1;
	if (n / 10)
		idx = ltoa_recursive(result, n / 10);
	else if (n < 0)
		result[idx++] = '-';
	if (n < 0)
		sign = -1;
	result[idx++] = '0' + sign * (n % 10);
	result[idx] = '\0';
	return (idx);
}
