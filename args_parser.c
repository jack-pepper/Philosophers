#include "philo.h"

int     args_are_valid(int argc, char **argv, t_philo_vars *settings)
{
    // check number of args
    if (argv[1])
    // check that only digits (find an exception for "+"?
    

    // check that bounds of the number fits the needed type and limits


    return (0);
}

// NB: will need a ft_atouint_64_t function for storing timestamps
void    store_args(char **argv, t_philo_vars *settings)
{
    settings->number_of_philosophers = ft_atonum_unsigned(argv[1], "int");
    settings->time_to_die = ft_atonum_unsigned(argv[2], "int64_t"); // Such a large type is probably non needed...
    settings->time_to_eat = ft_atonum_unsigned(argv[3], "int64_t");
    settings->time_to_sleep = ft_atonum_unsigned(argv[4], "int64_t");
    if (argv[5])
        settings->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
    return ;
}