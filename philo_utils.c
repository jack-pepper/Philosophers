#include "philo.h"

int	ft_isspace(int c)
{
	return (c == 32 || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v');
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (n && *s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}


int ft_atoi(const char *nptr)
{
    int result = 0;
    int sign = 1;
    
    while (ft_isspace(*nptr))
    nptr++;
    if (*nptr == '+' || *nptr == '-')
    {
        if (*nptr == '-')
        sign = -1;
        nptr++;
    }
    while (ft_isdigit(*nptr))
    {
        result = (result * 10) + (*nptr - '0');
        nptr++;
    }
    return (result * sign);
}

size_t	ft_strlen(const char *s)
{
    const char	*str;

    str = s;
    while (*str)
    {
        str++;
    }
    return (str - s);
}