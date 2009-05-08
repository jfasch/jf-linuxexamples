// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008 Joerg Faschingbauer

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

#include "timeval.h"

#include <cassert>

namespace jflinux {

TimeVal::TimeVal(long sec, long usec) {
    assert(sec>=0);
    assert(usec>=0);
    tv_sec = sec;
    tv_usec = usec;
    while (tv_usec >= one_second) {
        tv_sec++;
        tv_usec -= one_second;
    }
}

bool TimeVal::operator<(const TimeVal& t) const {
   if (tv_sec < t.tv_sec)
      return true;
   else if (tv_sec == t.tv_sec  &&  tv_usec < t.tv_usec)
      return true;
   else 
      return false;
}

bool TimeVal::operator>(const TimeVal& t) const {
   if (tv_sec > t.tv_sec)
      return true;
   else if (tv_sec == t.tv_sec  &&  tv_usec > t.tv_usec)
      return true;
   else 
      return false;
}

bool TimeVal::operator==(const TimeVal& t) const {
   return tv_sec == t.tv_usec  &&  tv_usec == t.tv_usec;
}

TimeVal TimeVal::now() {
    TimeVal tv;
    int err = ::gettimeofday(&tv, 0);
//     timespec ts;
//     int err = ::clock_gettime(CLOCK_REALTIME, &ts);
    assert(!err);
    return tv;
}

TimeVal operator+(const TimeVal& t1, const TimeVal& t2) {
    TimeVal sum;
    sum.tv_sec = t1.tv_sec + t2.tv_sec;
    sum.tv_usec = t1.tv_usec + t2.tv_usec;
    if (sum.tv_usec >= TimeVal::one_second) {
        sum.tv_usec -= TimeVal::one_second;
        sum.tv_sec++;
    }
    else if (sum.tv_sec >= 1 && sum.tv_usec < 0) {
        sum.tv_usec += TimeVal::one_second;
        sum.tv_sec--;
    }
    return sum;   
}

TimeVal operator-(const TimeVal& t1, const TimeVal& t2) {
    TimeVal delta;
    delta.tv_sec = t1.tv_sec - t2.tv_sec;
    delta.tv_usec = t1.tv_usec - t2.tv_usec;
    if (delta.tv_usec < 0) {
        delta.tv_usec += TimeVal::one_second;
        delta.tv_sec--;
    }
    else if (delta.tv_usec >= TimeVal::one_second) {
        delta.tv_usec -= TimeVal::one_second;
        delta.tv_sec++;
    }
    return delta;
}

}
