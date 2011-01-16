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

#ifndef HAVE_JF_LINUXTOOLS_TIMESPEC_H
#define HAVE_JF_LINUXTOOLS_TIMESPEC_H

#include <exception>
#include <sys/time.h>

namespace jf {
namespace linuxtools {

class TimeSpec : public timespec
{
public:
    enum Inf { Infinity };
    class UnderflowError : public std::exception
    {
    public:
        virtual const char* what() { return "TimeSpec::UnderflowError"; }
    };
    
public:
    /** Infinity */
    TimeSpec(Inf) { tv_sec = -1; tv_nsec = 0; }
    /** Initialize to zero */
    TimeSpec() { tv_sec = 0; tv_nsec = 0; }
    /** Initialize timespec structure with seconds and nanoseconds */
    TimeSpec(long sec, long nsec);
    /** Initialize from a timespec structure that the user might have
     * gotten from somewhere */
    TimeSpec(const timespec& t) { tv_sec = t.tv_sec; tv_nsec = t.tv_nsec; }
    
    long sec() const { return tv_sec; }
    long nsec() const { return tv_nsec; }
    double secs() const { return tv_sec+(double)tv_nsec/(double)one_second; }
    
    bool is_infinite() const { return tv_sec == -1; }
    static TimeSpec infinity() { return TimeSpec(Infinity); }

    bool operator<(const TimeSpec&) const;
    bool operator==(const TimeSpec&) const;
    bool operator!=(const TimeSpec& t) const { return !operator==(t); }
    bool operator>(const TimeSpec&) const;
    bool operator<=(const TimeSpec& t) const { return !operator>(t); }
    bool operator>=(const TimeSpec& t) const { return !operator<(t); }
    TimeSpec& operator+=(const TimeSpec&);

    /** Current point in time (as given by gettimeofday(2)) */
    static TimeSpec now_timeofday();
    /** Current point in time (as given by clock_gettime(2),
        CLOCK_MONOTONIC) */
    static TimeSpec now_monotonic();

    enum { one_second = 1000000000 /*nanoseconds*/ };
};

TimeSpec operator+(const TimeSpec&, const TimeSpec&);
TimeSpec operator-(const TimeSpec&, const TimeSpec&);

}
}

#endif
