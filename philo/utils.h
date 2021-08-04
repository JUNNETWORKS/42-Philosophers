#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>

int		ft_isdigit(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
long	ft_atol(const char *nptr, long *num);
long	get_current_time_ms(void);
void	precise_sleep_ms(long ms);
int		ltoa_recursive(char *result, long n);
void	ft_putnbr_fd(long n, int fd);

#endif
