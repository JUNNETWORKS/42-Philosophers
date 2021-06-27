#include <stdbool.h>
#include "utils.h"

static int	num_len(const char *str)
{
	int	i;

	i = 0;
	while (ft_isdigit(*str++))
		i++;
	return (i);
}

bool	is_int_overflow(const char *nptr, int sign)
{
	if (num_len(nptr) > 10)
		return (true);
	if (num_len(nptr) == 10)
	{
		if (sign == -1 && ft_strncmp(nptr, "2147483648", 10) > 0)
			return (true);
		if (sign == 1 && ft_strncmp(nptr, "2147483647", 10) > 0)
			return (true);
	}
	return (false);
}

int	ft_atoi(const char *nptr, int *num)
{
	unsigned int	tmp;
	int				np;

	np = 1;
	tmp = 0;
	while (*nptr == ' ' || *nptr == '\t' || *nptr == '\f'
		|| *nptr == '\r' || *nptr == '\n' || *nptr == '\v')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			np = -1;
	if (!ft_isdigit(*nptr) || is_int_overflow(nptr, np))
		return (-1);
	while (ft_isdigit(*nptr))
		tmp = tmp * 10 + (*nptr++ - '0');
	*num = (int)(np * tmp);
	return (0);
}
