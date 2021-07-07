#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

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
	long	num_of_philos;  // 哲学者の数  number_of_philosophers
	long	time_to_die_ms;      // 最後に食事した時間から time_to_die_ms 経つと死ぬ
	long	time_to_eat_ms;      // 食事にかかる時間[ms]
	long	time_to_sleep_ms;    // 睡眠にかかる時間[ms]
	// 全哲学者が’number_of_times_each_philosopher_must_eat’回食事をしたらシミュレーション終了.
	// 指定されていない場合は全哲学者が死ぬまでシミュレーションが続く
	long	must_eat_times;  // number_of_times_each_philosopher_must_eat
} t_philos_info;

typedef struct s_philo {
	enum e_philo_status	status;
	long				last_eating_ms;  // 最後に食べた時間
	int					eating_count;
	bool				is_living;
	pthread_t			thread;
} t_philo;

t_philos_info	g_philos_info;
t_philo			*g_philos;
pthread_mutex_t	*g_forks;

int		start_g_philos(void);
int		parse_philos_argv(int argc, char **argv);
int		monitor_if_philosophers_are_living(void);
bool	is_philo_still_alive(int philo_idx);
int		philosopher_eat(int philo_idx);
int		philosopher_sleep(int philo_idx);
void	write_philo_status(int philo_idx, enum e_philo_status status, long ms);
// fork
int		init_g_forks(int fork_num);
void	hold_fork(int fork_idx);
void	release_fork(int fork_idx);

#endif
