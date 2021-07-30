#include <unistd.h>
#include "utils.h"

void	precise_sleep_ms(long ms)
{
	long	current_time_ms;

	current_time_ms = get_current_time_ms();
	while (get_current_time_ms() - current_time_ms < ms)
		usleep(100);
}
