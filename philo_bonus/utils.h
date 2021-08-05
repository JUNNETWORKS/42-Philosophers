#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>

int		ft_isdigit(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
long	ft_atol(const char *nptr, long *num);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	*ft_ltoa(long n);
void	*ft_calloc(size_t nmemb, size_t size);
void	*free_and_rtn_null(void *ptr);
long	get_current_time_ms(void);
long	get_current_time_us(void);
void	precise_sleep_ms(long ms);
void	precise_sleep_until_ms(long ms);
void	precise_sleep_us(long us);
void	precise_sleep_until_us(long us);
long	arr_find_pid(pid_t *arr, pid_t value, long len);

#endif
