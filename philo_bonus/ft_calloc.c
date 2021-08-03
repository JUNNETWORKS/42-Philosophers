#include "utils.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*allocation;
	size_t			i;

	allocation = malloc(nmemb * size);
	if (!allocation)
		return (NULL);
	i = 0;
	while (i < nmemb * size)
		allocation[i++] = 0;
	return ((void *)allocation);
}
