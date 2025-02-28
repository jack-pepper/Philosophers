#include "philo.h"

signed char ft_atoc_sig(const char *nptr)
{
	signed char     result;
	signed char	    sign;

	result = 0;
	sign = 1;
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

unsigned char	ft_atoc_uns(const char *nptr)
{
	unsigned char	result;

	result = 0;
	while (ft_isspace(*nptr))
		nptr++;
    if (*nptr == '+')
        nptr++;
	else if (*nptr == '-')
        return (0);
    while (ft_isdigit(*nptr))
	{
		result = (result * 10) + (*nptr - '0');
		nptr++;
	}
	return (result);
}