#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>

int		ft_isdigit(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_atol(const char *nptr, long *num);
long	get_current_time_ms(void);
void	precise_sleep_ms(long ms);
int	arr_find_pid(pid_t *arr, pid_t value, int len);

#endif
