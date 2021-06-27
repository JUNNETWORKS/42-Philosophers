#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>

enum e_philo_status {
	HAS_TAKEN_A_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
};

typedef struct s_philos_info {
	int	num_of_philos;  // 哲学者の数  number_of_philosophers
	int	time_to_die_ms;      // 最後に食事した時間から time_to_die_ms 経つと死ぬ
	int	time_to_eat_ms;      // 食事にかかる時間[ms]
	int	time_to_sleep_ms;    // 睡眠にかかる時間[ms]
	// 全哲学者が’number_of_times_each_philosopher_must_eat’回食事をしたらシミュレーション終了.
	// 指定されていない場合は全哲学者が死ぬまでシミュレーションが続く
	int	must_eat_times;  // number_of_times_each_philosopher_must_eat
} t_philos_info;

typedef struct s_philo {
	enum e_philo_status	status;
	long				last_eating_ms;  // 最後に食べた時間
} t_philo;

t_philos_info	g_philos_info;
t_philo			*g_philos;
pthread_mutex_t	*g_forks;
pthread_t		*g_philo_thrs;
pthread_mutex_t	g_stdout_mutex;

void	*thr_philosopher(void *arg);
int		parse_philos_argv(int argc, char **argv);
int		philospher_eat(int philo_idx);
int		philospher_sleep(int philo_idx);
int		philospher_think(int philo_idx);
void	write_philo_status(int philo_no, long ms, enum e_philo_status status);
// fork
int		init_g_forks(int fork_num);
void	hold_fork(int fork_idx);
void	release_fork(int fork_idx);

#endif
