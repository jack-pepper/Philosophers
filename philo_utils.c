#include "philo.h"

//must display timestamp_in_ms X status
// on status change, send the full text: has taken a fork, is eating, is sleeping, is thinking, died
void update_state (t_philosopher *philosopher, char *new_state)
{
    uint64_t timestamp_ms;

    printf("%ld Philosopher %d %s.\n", timestamp_ms, philosopher->id, new_state);
}