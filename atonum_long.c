#include "philo.h"

signed long ft_atol_sig(const char *nptr)
{
    signed long result = 0;
    signed long sign = 1;

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

unsigned long	ft_atol_uns(const char *nptr)
{
	unsigned long	result;

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