#include <sys/time.h>
#include "utils.h"

long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec / 1000 + tv.tv_usec * 1000);
}
