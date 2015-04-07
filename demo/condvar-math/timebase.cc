#include "timebase.h"

#include <time.h>


Timebase::Timebase(
    ChannelGroup& channels,
    const jf::linuxtools::TimeSpec& interval)
: channels_(channels),
  interval_(interval) {}

void Timebase::run()
{
    for (;;) {
        for (size_t i=0; i<channels_.nchannels(); i++) {
            Channel::Ptr channel = channels_.get(i);
            channel->halt();
        }

        int err = nanosleep(&interval_, NULL);
        assert(!err);
    }
}
