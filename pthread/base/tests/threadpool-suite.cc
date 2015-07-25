// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2015 Joerg Faschingbauer

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

#include "threadpool-suite.h"

#include <jf/linuxtools/threadpool.h>
#include <jf/unittest/case.h>

#include <unistd.h>


namespace {
    class MySpecialTestCommand : public jf::linuxtools::ThreadPool::Command
    {
    public:
        MySpecialTestCommand() : is_done_(false) {}
        virtual void execute() { is_done_ = true; }
        bool is_done() const { return is_done_; }
    private:
        bool is_done_;
    };
}

namespace jf {
namespace linuxtools {

class ThreadPoolTest : public jf::unittest::TestCase
{
public:
    ThreadPoolTest() : jf::unittest::TestCase("ThreadPool") {}
    virtual void run();
};

void ThreadPoolTest::run()
{
    ThreadPool thread_pool(2);
    MySpecialTestCommand test_command;
    thread_pool.execute(&test_command);
    JFUNIT_ASSERT(test_command.is_done());
}

ThreadPoolSuite::ThreadPoolSuite()
: jf::unittest::TestSuite("ThreadPool")
{
    add_test(std::auto_ptr<Test>(new ThreadPoolTest));
}

}
}
