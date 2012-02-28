// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2011-2012 Joerg Faschingbauer

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

#include "worker-thread-suite.h"

#include <jf/linuxtools/worker-thread.h>

#include <jf/unittest/case.h>

namespace {

using namespace jf::linuxtools;

class ExecuteAsyncTest : public jf::unittest::TestCase
{
public:
    ExecuteAsyncTest()
    : jf::unittest::TestCase("ExecuteAsync"),
      thread_(5) {}

    virtual void setup()
    {
        thread_.start();
    }
    virtual void teardown()
    {
        thread_.stop();
    }
    virtual void run()
    {
        class MyWork : public Work
        {
        public:
            MyWork(Future<bool>* done) : done_(done) {}
            virtual void execute() { done_->set(true); }
        private:
            Future<bool>* done_;
        };


        Future<bool> done;
        thread_.execute_work_async(std::auto_ptr<Work>(new MyWork(&done)));

        // synchronise with work execution.
        done.get();
    }

private:
    WorkerThread thread_;
};

class ExecuteSyncTest : public jf::unittest::TestCase
{
public:
    ExecuteSyncTest()
    : jf::unittest::TestCase("ExecuteSync"),
      thread_(5) {}

    virtual void setup()
    {
        thread_.start();
    }
    virtual void teardown()
    {
        thread_.stop();
    }
    virtual void run()
    {
        class MyWork : public Work
        {
        public:
            MyWork() : done_(false) {}
            bool done() const { return done_; }
            virtual void execute() { done_ = true; }
        private:
            bool done_;
        };


        MyWork work;
        JFUNIT_ASSERT(!work.done());
        thread_.execute_work_sync(work);
        JFUNIT_ASSERT(work.done());
    }
    
private:
    WorkerThread thread_;
};

}

namespace jf {
namespace linuxtools {

WorkerThreadSuite::WorkerThreadSuite()
: jf::unittest::TestSuite("WorkerThread")
{
    add_test(std::auto_ptr<Test>(new ExecuteAsyncTest));
    add_test(std::auto_ptr<Test>(new ExecuteSyncTest));
}

}
}
