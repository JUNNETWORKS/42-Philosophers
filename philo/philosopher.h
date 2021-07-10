#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>
# include <stdbool.h>

/*
 * EATING -> THINKING -> SLEEPING -> EATING -> ...
 */
enum e_philo_status {
	HAS_TAKEN_A_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
};

typedef struct s_philos_info {
	long	num_of_philos;
	long	time_to_die_ms;
	long	time_to_eat_ms;
	long	time_to_sleep_ms;
	long	must_eat_times;
}	t_philos_info;

typedef struct s_philo {
	enum e_philo_status	status;
	long				last_eating_ms;
	int					eating_count;
	bool				is_living;
	pthread_t			thread;
}	t_philo;

extern t_philos_info	g_philos_info;
extern t_philo			*g_philos;
extern pthread_mutex_t	*g_forks;

int		start_g_philos(void);
int		parse_philos_argv(int argc, char **argv, t_philos_info *philos_info);
int		monitor_if_philosophers_are_living(void);
bool	is_philo_still_alive(int philo_idx);
int		philosopher_eat(int philo_idx);
int		philosopher_sleep(int philo_idx);
void	write_philo_status(int philo_idx, enum e_philo_status status, long ms);
// philosopher observer
int		start_philo_observers(pthread_t	*philo_observers);
int		wait_philo_observers(pthread_t	*philo_observers);
// fork
int		init_g_forks(int fork_num);
void	hold_fork(int fork_idx);
void	release_fork(int fork_idx);

#endif
