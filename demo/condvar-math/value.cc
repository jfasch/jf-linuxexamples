#include "value.h"


// ----------------------------------------------------
ChannelValue::ChannelValue(
    const Channel::Ptr& channel)
: channel_(channel) {}

double ChannelValue::get()
{
    return channel_->get();
}

// ----------------------------------------------------
PlusValue::PlusValue(Value& lhs, Value& rhs)
: lhs_(lhs),
  rhs_(rhs) {}

double PlusValue::get()
{
    return lhs_.get() + rhs_.get();
}

// ----------------------------------------------------
MinusValue::MinusValue(Value& lhs, Value& rhs)
: lhs_(lhs),
  rhs_(rhs) {}

double MinusValue::get()
{
    return lhs_.get() - rhs_.get();
}

// ----------------------------------------------------
MultValue::MultValue(Value& lhs, Value& rhs)
: lhs_(lhs),
  rhs_(rhs) {}

double MultValue::get()
{
    return lhs_.get() * rhs_.get();
}

// ----------------------------------------------------
DivValue::DivValue(Value& lhs, Value& rhs)
: lhs_(lhs),
  rhs_(rhs) {}

double DivValue::get()
{
    return lhs_.get() / rhs_.get();
}
