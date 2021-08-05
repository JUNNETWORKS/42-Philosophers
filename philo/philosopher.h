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
	HAS_EATEN,
};

typedef struct s_philos_info {
	pthread_mutex_t	mux;
	long			num_of_philos;
	long			time_to_die_ms;
	long			time_to_eat_ms;
	long			time_to_sleep_ms;
	long			must_eat_times;
	bool			end_of_simulation;
	pthread_mutex_t	*forks;
	pthread_mutex_t	writing_mux;
}	t_philos_info;

typedef struct s_philo {
	long				idx;
	enum e_philo_status	status;
	long				last_eating_ms;
	long				eating_count;
	pthread_mutex_t		mux;
	pthread_t			thread;
	t_philos_info		*philos_info;
}	t_philo;

int		parse_philos_argv(int argc, char **argv, t_philos_info *philos_info);
int		init_philos(t_philos_info *philos_info, t_philo *philos);
int		start_philos(t_philos_info *philos_info, t_philo *philos);
int		monitor_if_philosophers_are_living(void);
bool	is_philo_simulation_ended(t_philos_info *philos_info);
bool	is_philo_still_alive(t_philo *philo);
bool	has_philo_eaten_n_times(t_philo *philo);
int		philosopher_eat(t_philos_info *philos_info, t_philo *philo);
int		philosopher_sleep(t_philos_info *philos_info, t_philo *philo);
void	write_philo_status(pthread_mutex_t *writing_mux,
			long philo_idx, enum e_philo_status status);
// philosopher observer
void	*thr_philo_observer(void *arg);
int		start_philo_observers(t_philos_info *philos_info,
			t_philo *philos, pthread_t *philo_observers);
int		wait_philo_observers(t_philos_info *philos_info,
			t_philo *philos);
// fork
int		init_forks(pthread_mutex_t *forks, long fork_num);
void	hold_fork(pthread_mutex_t *forks, long fork_idx);
void	release_fork(pthread_mutex_t *forks, long fork_idx);
void	release_forks(pthread_mutex_t *forks, long fork_idx1, long fork_idx2);
int		release_forks_and_rtn_err(pthread_mutex_t *forks,
			long fork_idx1, long fork_idx2);

#endif
