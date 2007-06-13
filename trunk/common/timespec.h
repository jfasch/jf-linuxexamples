// -*- mode: C++; c-basic-offset: 4 -*-

#ifndef jf_unix_tools_timespec_h
#define jf_unix_tools_timespec_h

#include <time.h>

namespace jf {
namespace unix_tools {

class TimeSpec : public timespec {
public:
    enum Inf { Infinity };
    
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
    
    bool is_infinite() const;

    bool operator<(const TimeSpec&) const;
    bool operator==(const TimeSpec&) const;
    bool operator>(const TimeSpec&) const;
    bool operator<=(const TimeSpec& t) const { return !operator>(t); }
    bool operator>=(const TimeSpec& t) const { return !operator<(t); }

    /** Current point in time */
    static TimeSpec now();

    enum { one_second = 1000000000 /*nanoseconds*/ };
};

inline bool TimeSpec::is_infinite() const {
    return tv_sec == -1;
}

TimeSpec operator+(const TimeSpec&, const TimeSpec&);
TimeSpec operator-(const TimeSpec&, const TimeSpec&);

}
}

#endif
