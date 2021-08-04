#include <unistd.h>
#include "utils.h"

void	ft_putnbr_fd(long n, int fd)
{
	char		result[20];
	int			len;

	len = ltoa_recursive(result, n);
	write(fd, result, len + 1);
}

