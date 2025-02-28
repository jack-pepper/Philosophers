#include "philo.h"

signed short ft_atos_sig(const char *nptr)
{
    signed short result = 0;
    signed short sign = 1;

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

unsigned short	ft_atos_uns(const char *nptr)
{
	unsigned short	result;

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