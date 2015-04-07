#include "producer.h"

#include <time.h>


Producer::Producer(
    const Channel::Ptr& ch,
    double start,
    double step,
    const jf::linuxtools::TimeSpec& interval)
: channel_(ch),
  next_(start),
  step_(step),
  interval_(interval) {}

void Producer::run()
{
    for (;;) {
        channel_->set_and_kick(next_);
        next_ += step_;
        int err = nanosleep(&interval_, NULL);
        assert(!err);
    }
}
