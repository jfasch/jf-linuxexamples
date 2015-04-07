#include "channel.h"

Channel::Channel()
: is_valid_(false) {}

double Channel::get() const
{
    mutex_.lock();
    while (!is_valid_)
        cond_is_valid_.wait(mutex_);
    double value = value_;
    mutex_.unlock();

    return value_;
}

void Channel::set_and_kick(double value)
{
    mutex_.lock();
    value_ = value;
    is_valid_ = true;
    mutex_.unlock();

    cond_is_valid_.signal();
}

void Channel::halt()
{
    mutex_.lock();
    is_valid_ = false;
    mutex_.unlock();
}
