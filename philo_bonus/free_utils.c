#include <stdlib.h>
#include "utils.h"

void	*free_and_rtn_null(void *ptr)
{
	free(ptr);
	return (NULL);
}
