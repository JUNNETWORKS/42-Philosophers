#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include "philosopher.h"
#include "utils.h"

t_philos_info	g_philos_info;

int	main(int argc, char **argv)
{
	pid_t			*philos_pid;
	sem_t			*forks;

	if (parse_philos_argv(argc, argv, &g_philos_info))
		return (1);
	philos_pid = malloc(g_philos_info.num_of_philos);
	if (!philos_pid)
		return (1);

	// フォークを表すセマフォを作成
	// O_CREAT | O_EXCL で確実にセマフォを作成する. 既に同名のセマフォが存在している場合はエラー.
	forks = sem_open(SEM_STR, O_CREAT | O_EXCL, S_IRWXU, g_philos_info.num_of_philos);
	// 哲学者のプロセスを作成
	start_philo_processes(g_philos_info, philos_pid);
	// 哲学者のプロセスが全て終了するまで待つ
	wait_philo_processes(g_philos_info, philos_pid);
	// セマフォを削除
	sem_unlink(SEM_STR);
	return (0);
}
