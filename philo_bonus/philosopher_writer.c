#include <stdio.h>
#include <semaphore.h>
#include "philosopher.h"
#include "utils.h"

/* {timestamp_in_ms} {philo_no} {status_msg} */
void	write_philo_status(
	sem_t *writing_sem, long philo_idx, enum e_philo_status status)
{
	long		philo_no;

	philo_no = philo_idx + 1;
	sem_wait(writing_sem);
	if (status == HAS_TAKEN_A_FORK)
		printf("%ld %ld has taken a fork\n", get_current_time_ms(), philo_no);
	else if (status == EATING)
		printf("%ld %ld is eating\n", get_current_time_ms(), philo_no);
	else if (status == SLEEPING)
		printf("%ld %ld is sleeping\n", get_current_time_ms(), philo_no);
	else if (status == THINKING)
		printf("%ld %ld is thinking\n", get_current_time_ms(), philo_no);
	else if (status == DIED)
		printf("%ld %ld died\n", get_current_time_ms(), philo_no);
	else if (status == HAS_EATEN)
		printf("%ld %ld has eaten completely\n",
			get_current_time_ms(), philo_no);
	if (status != DIED)
		sem_post(writing_sem);
}

