#include "philo.h"

int64_t ft_atoint64_sig(const char *nptr)
{
    int64_t result;
    int64_t sign;

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

uint64_t    ft_atoint64_uns(const char *nptr)
{
	uint64_t	result;

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