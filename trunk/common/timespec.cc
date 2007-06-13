// -*- mode: C++; c-basic-offset: 4 -*-

#include "timespec.h"

#include <cassert>
#include <sys/time.h>

namespace jf {
namespace unix_tools {

TimeSpec::TimeSpec(long sec, long nsec) {
    assert(sec>=0);
    assert(nsec>=0);
    tv_sec = sec;
    tv_nsec = nsec;
    while (tv_nsec >= one_second) {
        tv_sec++;
        tv_nsec -= one_second;
    }
}

bool TimeSpec::operator<(const TimeSpec& t) const {
   if (tv_sec < t.tv_sec)
      return true;
   else if (tv_sec == t.tv_sec  &&  tv_nsec < t.tv_nsec)
      return true;
   else 
      return false;
}

bool TimeSpec::operator>(const TimeSpec& t) const {
   if (tv_sec > t.tv_sec)
      return true;
   else if (tv_sec == t.tv_sec  &&  tv_nsec > t.tv_nsec)
      return true;
   else 
      return false;
}

bool TimeSpec::operator==(const TimeSpec& t) const {
   return tv_sec == t.tv_nsec  &&  tv_nsec == t.tv_nsec;
}

TimeSpec TimeSpec::now() {
    struct timeval tv;
    int err = ::gettimeofday(&tv, NULL);
//     timespec ts;
//     int err = ::clock_gettime(CLOCK_REALTIME, &ts);
    assert(!err);
    return TimeSpec(tv.tv_sec, tv.tv_usec*1000);
}

TimeSpec operator+(const TimeSpec& t1, const TimeSpec& t2) {
    TimeSpec sum;
    sum.tv_sec = t1.tv_sec + t2.tv_sec;
    sum.tv_nsec = t1.tv_nsec + t2.tv_nsec;
    if (sum.tv_nsec >= TimeSpec::one_second) {
        sum.tv_nsec -= TimeSpec::one_second;
        sum.tv_sec++;
    }
    else if (sum.tv_sec >= 1 && sum.tv_nsec < 0) {
        sum.tv_nsec += TimeSpec::one_second;
        sum.tv_sec--;
    }
    return sum;   
}

TimeSpec operator-(const TimeSpec& t1, const TimeSpec& t2) {
    TimeSpec delta;
    delta.tv_sec = t1.tv_sec - t2.tv_sec;
    delta.tv_nsec = t1.tv_nsec - t2.tv_nsec;
    if (delta.tv_nsec < 0) {
        delta.tv_nsec += TimeSpec::one_second;
        delta.tv_sec--;
    }
    else if (delta.tv_nsec >= TimeSpec::one_second) {
        delta.tv_nsec -= TimeSpec::one_second;
        delta.tv_sec++;
    }
    return delta;
}

}
}
