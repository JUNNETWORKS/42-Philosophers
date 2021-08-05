#include <unistd.h>
#include "utils.h"

void	precise_sleep_ms(long ms)
{
	long	current_time_ms;

	current_time_ms = get_current_time_ms();
	while (get_current_time_ms() - current_time_ms < ms)
		usleep(1000);
}

void	precise_sleep_until_ms(long ms)
{
	while (get_current_time_ms() < ms)
		usleep(1000);
}

void	precise_sleep_us(long us)
{
	long	current_time_us;

	current_time_us = get_current_time_us();
	while (get_current_time_us() - current_time_us < us)
		usleep(1000);
}

void	precise_sleep_until_us(long us)
{
	while (get_current_time_us() < us)
		usleep(1000);
}
