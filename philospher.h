#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>

typedef struct s_philos_info {
	int	num_of_philos;  // 哲学者の数  number_of_philosophers
	int	time_to_die_ms;      // 最後に食事した時間から time_to_die_ms 経つと死ぬ
	int	time_to_eat_ms;      // 食事にかかる時間[ms]
	int	time_to_sleep_ms;    // 睡眠にかかる時間[ms]
	// 全哲学者が’number_of_times_each_philosopher_must_eat’回食事をしたらシミュレーション終了.
	// 指定されていない場合は全哲学者が死ぬまでシミュレーションが続く
	int	must_eat_times;  // number_of_times_each_philosopher_must_eat
} t_philos_info;

t_philos_info	g_philos_info;
pthread_mutex_t	*g_folks;
pthread_t		*g_philos;

void	*thr_philosopher(void *arg);
int		parse_philos_argv(int argc, char **argv);

#endif
