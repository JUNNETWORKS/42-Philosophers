#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

/* {timestamp_in_ms} {philo_no} {status_msg} */
void	write_philo_status(long philo_idx, enum e_philo_status status, long ms)
{
	long		philo_no;

	philo_no = philo_idx + 1;
	if (status == HAS_TAKEN_A_FORK)
		printf("%ld %ld has taken a fork\n", ms, philo_no);
	else if (status == EATING)
		printf("%ld %ld is eating\n", ms, philo_no);
	else if (status == SLEEPING)
		printf("%ld %ld is sleeping\n", ms, philo_no);
	else if (status == THINKING)
		printf("%ld %ld is thinking\n", ms, philo_no);
	else if (status == DIED)
		printf("%ld %ld died\n", ms, philo_no);
	else if (status == HAS_EATEN)
		printf("%ld %ld has eaten completely\n", ms, philo_no);
}
