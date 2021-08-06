#include <unistd.h>
#include "utils.h"

void	precise_sleep_ms(long ms)
{
	long	current_time_us;

	current_time_us = get_current_time_us();
	while (get_current_time_us() - current_time_us < ms * 1000)
		usleep(1000);
}

void	precise_sleep_until_ms(long ms)
{
	while (get_current_time_us() < ms * 1000)
		usleep(1000);
}
