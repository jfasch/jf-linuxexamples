// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2010-2011 Joerg Faschingbauer

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

#include "dispatcher_suite.h"

#include <jf/linuxtools/dispatcher.h>
#include <jf/linuxtools/socketpair.h>
#include <jf/linuxtools/active_object.h>

#include <jf/unittest/test_case.h>

namespace {

using namespace jf::linuxtools;

class DispatcherBasicTest : public jf::unittest::TestCase
{
public:
    DispatcherBasicTest() : jf::unittest::TestCase("Basic") {}

    virtual void setup() { handler_.activate(&dispatcher_); }
    virtual void teardown() { handler_.deactivate(&dispatcher_); }

    virtual void run()
    {
        while (handler_.seen() != 1)
            dispatcher_.dispatch();
    }

private:
    class MyHandler : public ActiveObject,
                      private Dispatcher::Handler {
    public:
        MyHandler() : seen_(0) {}
        void activate(Dispatcher* d) {
            assert(dispatcher_==NULL);
            dispatcher_ = d;
            d->watch_in(channel_.left().fd(), this);
            d->watch_out(channel_.right().fd(), this);
        }
        void deactivate(const Dispatcher* d) {
            assert(dispatcher_!=NULL);
            assert(dispatcher_==d);
            dispatcher_->unwatch_in(channel_.left().fd(), this);
            dispatcher_->unwatch_out(channel_.right().fd(), this);
            dispatcher_ = NULL;
        }
        virtual void out_ready(int fd) {
            const char x = 1;
            channel_.right().write(&x, 1);
        }
        virtual void in_ready(int fd) {
            channel_.left().read(&seen_, 1);
        }
        char seen() const { return seen_; }
    private:
        Dispatcher* dispatcher_;
        SocketPair channel_;
        char seen_; 
    };

    Dispatcher dispatcher_;
    MyHandler handler_;
};

}

namespace jf {
namespace linuxtools {

DispatcherSuite::DispatcherSuite()
: jf::unittest::TestSuite("Dispatcher")
{
    add_test(new DispatcherBasicTest);
}

}
}
