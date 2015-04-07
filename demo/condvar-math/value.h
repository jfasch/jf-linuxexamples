#ifndef HAVE_JF_LINUXEXAMPLES__DEMO__CONDVAR_MATH__MATH_H
#define HAVE_JF_LINUXEXAMPLES__DEMO__CONDVAR_MATH__MATH_H

#include "channel.h"


class Value
{
public:
    virtual ~Value() {}
    virtual double get() = 0;
};

class ChannelValue : public Value
{
public:
    ChannelValue(const Channel::Ptr& channel);
    virtual double get();
private:
    Channel::Ptr channel_;
};

class ConstValue : public Value
{
public:
    ConstValue(double value) : value_(value) {}
    virtual double get() { return value_; }
private:
    double value_;
};

class PlusValue : public Value
{
public:
    PlusValue(Value& lhs, Value& rhs);
    virtual double get();
private:
    Value& lhs_;
    Value& rhs_;
};

class MinusValue : public Value
{
public:
    MinusValue(Value& lhs, Value& rhs);
    virtual double get();
private:
    Value& lhs_;
    Value& rhs_;
};

class MultValue : public Value
{
public:
    MultValue(Value& lhs, Value& rhs);
    virtual double get();
private:
    Value& lhs_;
    Value& rhs_;
};

class DivValue : public Value
{
public:
    DivValue(Value& lhs, Value& rhs);
    virtual double get();
private:
    Value& lhs_;
    Value& rhs_;
};

#endif
