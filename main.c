#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t	*folks;
pthread_t	*philos;

void	*thr_philosopher(void *arg)
{
	long	philo_no = (long)arg;
	printf("This is Philo %ld\n", philo_no);
	return ((void *)0);
}

int	main(void)
{
	int	num_of_philos = 5;  // 哲学者の数
	// int	time_to_die_ms = 100;      // 最後に食事した時間から time_to_die_ms 経つと死ぬ
	// int	time_to_eat_ms = 100;      // 食事にかかる時間[ms]
	// int	time_to_sleep_ms = 100;    // 睡眠にかかる時間[ms]
	// // 全哲学者が’number_of_times_each_philosopher_must_eat’回食事をしたらシミュレーション終了.
	// // 指定されていない場合は全哲学者が死ぬまでシミュレーションが続く
	// int	num_of_times_each_philo_must_eat = 10;

	folks = malloc(sizeof(pthread_mutex_t) * num_of_philos);
	philos = malloc(sizeof(pthread_t) * num_of_philos);
	if (!folks || !philos)
		return (1);

	long	i = 0;
	while (i < num_of_philos)
	{
		if (pthread_create(&philos[i], NULL, thr_philosopher, (void *)i))
		{
			printf("pthread_create() error!\n");
			return (1);
		}
		i++;
	}

	i = 0;
	while ( i < num_of_philos)
	{
		pthread_join(philos[i], NULL);
		i++;
	}

	printf("Hello Philosophers!!\n");
	return (0);
}
