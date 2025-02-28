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

char *ft_atonum_signed(const char *nptr, char *numtype)
{
    if (ft_strncmp(numtype, "char", sizeof("char") - 1))
        ft_atoc_sig(nptr);
    else if (ft_strncmp(numtype, "short", sizeof("short") - 1))
        ft_atos_sig(nptr);
    else if (ft_strncmp(numtype, "int", sizeof("int") - 1))
        ft_atoi_sig(nptr);
    else if (ft_strncmp(numtype, "long", sizeof("long") - 1))
        ft_atol_sig(nptr);
    else if (ft_strncmp(numtype, "long long", sizeof("long long") - 1))
        ft_atoll_sig(nptr);
    else if (ft_strncmp(numtype, "int64_t", sizeof("int64_t") - 1))
        ft_atoint64_t_sig(nptr);
    return (nptr);
}

char *ft_atonum_unsigned(const char *nptr, char *numtype)
{
    if (ft_strncmp(numtype, "char", sizeof("char") - 1))
        ft_atoc_uns(nptr);
    else if (ft_strncmp(numtype, "short", sizeof("short") - 1))
        ft_atos_uns(nptr);
    else if (ft_strncmp(numtype, "int", sizeof("int") - 1))
        ft_atoi_uns(nptr);
    else if (ft_strncmp(numtype, "long", sizeof("long") - 1))
        ft_atol_uns(nptr);
    else if (ft_strncmp(numtype, "long long", sizeof("long long") - 1))
        ft_atoll_uns(nptr);
    else if (ft_strncmp(numtype, "int64_t", sizeof("int64_t") - 1))
        ft_atoint64_t_uns(nptr);
    return (nptr);
}