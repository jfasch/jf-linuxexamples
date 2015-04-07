#include <jf/linuxtools/timespec.h>

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>
#include <errno.h>

#include <iostream>

using jf::linuxtools::TimeSpec;
using namespace std;

static const TimeSpec wait(0, TimeSpec::one_second/2);

int main(int argc, char** argv)
{
    cout << "using CLOCK_MONOTONIC to wait for " << wait.secs() << " seconds ..." << endl;
    
    int rv = mlockall(MCL_CURRENT|MCL_FUTURE);
    if (rv == 0) {
        sched_param param;
        memset(&param, 0, sizeof(param));
        param.sched_priority = 1; // should be enough for the show
        rv = sched_setscheduler(getpid(), SCHED_FIFO, &param);
        if (rv == -1) {
            perror("sched_setscheduler");
            exit(1);
        }
        cerr << "Doing realtime ..." << endl;
    }
    else {
        if (errno == EPERM || errno == ENOMEM)
            cerr << "Warning: cannot do realtime - apparently not privileged enough" << endl;
        else {
            perror("mlockall");
            exit(1);
        }
    }

    TimeSpec resolution;
    rv = clock_getres(CLOCK_MONOTONIC, &resolution);
    if (rv == -1) {
        perror("clock_getres");
        exit(1);
    }
    cout << "resolution: " << resolution.secs() << " seconds" << endl;

    for (;;) {
        TimeSpec before = TimeSpec::now_monotonic();

        TimeSpec when = before + wait;
        rv = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &when, NULL);
        if (rv == -1) {
            perror("clock_nanosleep");
            exit(1);
        }

        TimeSpec after = TimeSpec::now_monotonic();
        TimeSpec sleep_time = after - before;
        cout << "slept " << sleep_time.secs() << " seconds (latency " << (sleep_time-wait).secs() << ')' << endl;
    }
    return 0;
}
