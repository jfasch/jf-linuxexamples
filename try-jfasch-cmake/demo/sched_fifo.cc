// -*- mode: C++; c-basic-offset: 4 -*-

#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>

int main()
{
    sched_param param;
    memset(&param, 0, sizeof param);
    param.sched_priority = 10;

    if (sched_setscheduler(getpid(), SCHED_FIFO, &param) < 0) {
        perror("sched_setscheduler");
        return 1;
    }

    for (;;);

    return 0;
}
