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
t_philo			*g_philos;

int	main(int argc, char **argv)
{
	if (parse_philos_argv(argc, argv))
		return (1);
	// フォークを表すセマフォを作成
	// O_CREAT | O_EXCL で確実にセマフォを作成する. 既に同名のセマフォが存在している場合はエラー.
	sem_t	*forks = sem_open("/forks", O_CREAT | O_EXCL, S_IRWXU, g_philos_info.num_of_philos);
	// TODO: 哲学者のプロセスを作成
	
	// MEMO: フォークを取得 (セマフォの値を-1する)
	// セマフォ値が0だとブロックされる
	sem_wait(forks);
	// MEMO: フォークを解法 (セマフォの値を+1する)
	sem_post(forks);

	// セマフォを閉じる
	sem_close(forks);  // プロセス終了時には自動でセマフォをクローズしてくれるので別に無くてもいいかも
	// セマフォを削除
	sem_unlink("/forks");
	return (0);
}
