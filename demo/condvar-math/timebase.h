#ifndef HAVE_JF_LINUXEXAMPLES__DEMO__CONDVAR_MATH__TIMEBASE_H
#define HAVE_JF_LINUXEXAMPLES__DEMO__CONDVAR_MATH__TIMEBASE_H

#include "channel_group.h"

#include <jf/linuxtools/joinable-thread.h>
#include <jf/linuxtools/timespec.h>


class Timebase : public jf::linuxtools::JoinableThreadStarter::Worker
{
public:
    Timebase(
        ChannelGroup& channels,
        const jf::linuxtools::TimeSpec& interval);

private:
    virtual void run();

private:
    ChannelGroup& channels_;
    jf::linuxtools::TimeSpec interval_;
};

#endif
