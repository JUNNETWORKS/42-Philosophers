#include "utils.h"

long	get_digit(long n)
{
	long		digit;

	if (n == 0)
		return (1);
	digit = 0;
	while (n)
	{
		digit++;
		n /= 10;
	}
	return (digit);
}

long	itoa_recursive(char *result, long n)
{
	long	idx;
	long	sign;

	idx = 0;
	sign = 1;
	if (n / 10)
		idx = itoa_recursive(result, n / 10);
	else if (n < 0)
		result[idx++] = '-';
	if (n < 0)
		sign = -1;
	result[idx++] = '0' + sign * (n % 10);
	result[idx] = '\0';
	return (idx);
}

char	*ft_ltoa(long n)
{
	long	arr_size;
	char	*result;

	arr_size = get_digit(n) + 1;
	if (n < 0)
		arr_size++;
	result = (char *)ft_calloc(arr_size, sizeof(char));
	if (!result)
		return (NULL);
	itoa_recursive(result, n);
	return (result);
}
