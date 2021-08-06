#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>

# define PHILO_END_EATEN 0
# define PHILO_END_DIE 1
# define PHILO_END_ERR 2
# define SEM_FORKS_STR "/forks"
# define SEM_HAS_DIED_STR "/has_died"
# define SEM_WRITING_STR "/writing"

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
	long			num_of_philos;
	long			time_to_die_ms;
	long			time_to_eat_ms;
	long			time_to_sleep_ms;
	long			must_eat_times;
	sem_t			*forks;
	sem_t			*sem_writing;
	pid_t			*philo_pids;
}	t_philos_info;

typedef struct s_philo {
	long				idx;
	enum e_philo_status	status;
	long				last_eating_ms;
	long				eating_count;
	bool				is_living;
	sem_t				*sem;
	pthread_t			thread;
	t_philos_info		*philos_info;
}	t_philo;

// main process
int		parse_philos_argv(int argc, char **argv, t_philos_info *philos_info);
int		start_philos(t_philos_info *philos_info);
int		wait_philos(t_philos_info *philos_info);
// philosopher process
void	philo_process(t_philos_info *philos_info, long idx);
int		philosopher_eat(t_philos_info *philos_info, t_philo *philo);
int		philosopher_sleep(t_philos_info *philos_info, t_philo *philo);
bool	is_philo_simulation_ended(t_philos_info *philos_info, t_philo *philo);
bool	is_philo_still_alive(t_philo *philo);
bool	has_philo_eaten_n_times(t_philo *philo);
void	write_philo_status(
			sem_t *writing_sem, long philo_idx, enum e_philo_status status);
// observer
int		observe_philo(t_philos_info *philos_info, t_philo *philo);
// fork
void	hold_fork(sem_t *forks);
void	release_fork(sem_t *forks);
void	release_forks(sem_t *forks, long num);
int		release_forks_and_rtn_err(sem_t *forks, long num);

#endif
