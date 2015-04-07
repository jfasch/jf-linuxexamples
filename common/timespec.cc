// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2011 Joerg Faschingbauer

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2.1 of
// the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA

// CONFIX:REQUIRE_SYMBOL('jf.linuxexamples.externals.rt', REQUIRED)

#include "timespec.h"

#include <cassert>
#include <sys/types.h>
#include <time.h>

namespace jf {
namespace linuxtools {

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
   return tv_sec == t.tv_sec  &&  tv_nsec == t.tv_nsec;
}

TimeSpec& TimeSpec::operator+=(const TimeSpec& rhs)
{
    *this = *this + rhs;
    return *this;
}

TimeSpec TimeSpec::now_timeofday() {
    struct timeval tv;
    int err = ::gettimeofday(&tv, 0);
    assert(!err);
    return TimeSpec(tv.tv_sec, tv.tv_usec*1000);
}

TimeSpec TimeSpec::now_monotonic() {
    timespec ts;
    int err = ::clock_gettime(CLOCK_MONOTONIC, &ts);
    assert(!err);
    return TimeSpec(ts.tv_sec, ts.tv_nsec);
}

TimeSpec operator+(const TimeSpec& lhs, const TimeSpec& rhs)
{
    TimeSpec ret;
    ret.tv_sec = lhs.tv_sec + rhs.tv_sec;
    ret.tv_nsec = lhs.tv_nsec + rhs.tv_nsec;
    // take care of overflows
    ret.tv_sec += ret.tv_nsec / TimeSpec::one_second;
    ret.tv_nsec = ret.tv_nsec % TimeSpec::one_second;
    return ret;
}

TimeSpec operator-(const TimeSpec& lhs, const TimeSpec& rhs)
{
    bool debit = false;
    long ret_nsec;
    if (rhs.tv_nsec > lhs.tv_nsec) {
        debit = true;
        ret_nsec = TimeSpec::one_second - rhs.tv_nsec + lhs.tv_nsec;
    }
    else
        ret_nsec = lhs.tv_nsec - rhs.tv_nsec;

    long sub_sec = debit? rhs.tv_sec+1: rhs.tv_sec;
    if (sub_sec > lhs.tv_sec) {
        throw TimeSpec::UnderflowError();
    }

    return TimeSpec(lhs.tv_sec - sub_sec, ret_nsec);
}

}
}
