// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2010 Joerg Faschingbauer

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

#ifndef HAVE_JF_LINUXTOOLS_TIMEVAL_H
#define HAVE_JF_LINUXTOOLS_TIMEVAL_H

#include <sys/time.h>

namespace jf {
namespace linuxtools {

class TimeVal : public timeval {
public:
    enum Inf { Infinity };
    
public:
    /** Infinity */
    TimeVal(Inf) { tv_sec = -1; tv_usec = 0; }
    /** Initialize to zero */
    TimeVal() { tv_sec = 0; tv_usec = 0; }
    /** Initialize timeval structure with seconds and microseconds */
    TimeVal(long sec, long nsec);
    /** Initialize from a timeval structure that the user might have
        gotten from somewhere */
    TimeVal(const TimeVal& t) { tv_sec = t.tv_sec; tv_usec = t.tv_usec; }
    
    long sec() const { return tv_sec; }
    long usec() const { return tv_usec; }
    double secs() const { return tv_sec+(double)tv_usec/(double)one_second; }
    
    bool is_infinite() const;

    bool operator<(const TimeVal&) const;
    bool operator==(const TimeVal&) const;
    bool operator>(const TimeVal&) const;
    bool operator<=(const TimeVal& t) const { return !operator>(t); }
    bool operator>=(const TimeVal& t) const { return !operator<(t); }

    /** Current point in time (as given by gettimeofday(2)) */
    static TimeVal now();

    enum { one_second = 1000*1000 /*microseconds*/ };
};

inline bool TimeVal::is_infinite() const {
    return tv_sec == -1;
}

TimeVal operator+(const TimeVal&, const TimeVal&);
TimeVal operator-(const TimeVal&, const TimeVal&);

}
}

#endif
