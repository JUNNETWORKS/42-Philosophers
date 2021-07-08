#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>

# define SEM_STR "/forks"

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
	// 全哲学者が’number_of_times_each_philosopher_must_eat’回食事をしたらシミュレーション終了.
	// 指定されていない場合は全哲学者が死ぬまでシミュレーションが続く
	long	must_eat_times;
} t_philos_info;

typedef struct s_philo {
	long				idx;
	enum e_philo_status	status;
	long				last_eating_ms;
	int					eating_count;
	bool				is_living;
} t_philo;

int	start_philos_process(t_philos_info philos_info, pid_t *philos_pid);
int		parse_philos_argv(int argc, char **argv, t_philos_info *philos_info);
int		monitor_if_philosophers_are_living(void);
bool	is_philo_still_alive(t_philos_info philos_info, t_philo philo);
int		philosopher_eat(t_philos_info philos_info, t_philo *philo);
int		philosopher_sleep(t_philos_info philos_info, t_philo *philo);
void	write_philo_status(int philo_idx, enum e_philo_status status, long ms);
// philosopher process
void	philosopher_process(t_philos_info philos_info, long philo_idx);
// philosopher observer
int		start_philo_observers(pthread_t	*philo_observers);
int		wait_philo_observers(pthread_t	*philo_observers);
// fork
int		init_g_forks(int fork_num);
void	hold_fork(int fork_idx);
void	release_fork(int fork_idx);

#endif
