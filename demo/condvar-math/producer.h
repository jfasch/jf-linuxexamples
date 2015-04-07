#ifndef HAVE_JF_LINUXEXAMPLES__DEMO__CONDVAR_MATH__PRODUCER_H
#define HAVE_JF_LINUXEXAMPLES__DEMO__CONDVAR_MATH__PRODUCER_H

#include "channel.h"

#include <jf/linuxtools/joinable-thread.h>
#include <jf/linuxtools/timespec.h>


class Producer : public jf::linuxtools::JoinableThreadStarter::Worker
{
public:
    Producer(
        const Channel::Ptr& ch,
        double start,
        double step,
        const jf::linuxtools::TimeSpec& interval);

private:
    virtual void run();

private:
    Channel::Ptr channel_;
    double next_;
    double step_;
    jf::linuxtools::TimeSpec interval_;
};

#endif
