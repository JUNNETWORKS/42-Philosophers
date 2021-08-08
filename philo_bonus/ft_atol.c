#include <stdbool.h>
#include "utils.h"

static long	num_len(const char *str)
{
	long	i;

	i = 0;
	while (ft_isdigit(*str++))
		i++;
	return (i);
}

static long	is_long_overflow(const char *nptr, long sign)
{
	if (num_len(nptr) > 10)
		return (true);
	if (num_len(nptr) == 10)
	{
		if (sign == -1 && ft_strncmp(nptr, "9223372036854775808", 19) > 0)
			return (true);
		if (sign == 1 && ft_strncmp(nptr, "9223372036854775807", 19) > 0)
			return (true);
	}
	return (false);
}

long	ft_atol(const char *nptr, long *num)
{
	unsigned long	tmp;
	long			np;

	np = 1;
	tmp = 0;
	if (*nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			np = -1;
	if (!ft_isdigit(*nptr) || is_long_overflow(nptr, np))
		return (-1);
	while (ft_isdigit(*nptr))
		tmp = tmp * 10 + (*nptr++ - '0');
	if (*nptr != '\0')
		return (-1);
	*num = np * tmp;
	return (0);
}
