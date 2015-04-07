#ifndef HAVE_JF_LINUXEXAMPLES__DEMO__CONDVAR_MATH__CHANNEL_H
#define HAVE_JF_LINUXEXAMPLES__DEMO__CONDVAR_MATH__CHANNEL_H

#include <jf/linuxtools/mutex.h>
#include <jf/linuxtools/condition.h>

#include <boost/shared_ptr.hpp>


class Channel
{
public:
    typedef boost::shared_ptr<Channel> Ptr;

public:
    Channel();

    double get() const;

    void set_and_kick(double);
    void halt();

private:
    mutable jf::linuxtools::Mutex mutex_;
    mutable jf::linuxtools::Condition cond_is_valid_;

    bool is_valid_;
    double value_;
};

#endif
