#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include "philosopher.h"
#include "utils.h"

static void	set_philos_simulation_end(t_philo *philo)
{
	sem_t	*has_died;

	write_philo_status(philo->philos_info->sem_writing, philo->idx, DIED);
	has_died = sem_open(SEM_HAS_DIED_STR, O_EXCL, S_IRWXU);
	sem_post(has_died);
	sem_close(has_died);
	sem_wait(philo->sem);
	philo->is_living = false;
	sem_post(philo->sem);
}

/* Monitor if philosopher has died.
 */
int	observe_philo(t_philos_info *philos_info, t_philo *philo)
{
	(void)philos_info;
	while (true)
	{
		if (!is_philo_still_alive(philo))
		{
			set_philos_simulation_end(philo);
			return (PHILO_END_DIE);
		}
		usleep(1000);
	}
}
