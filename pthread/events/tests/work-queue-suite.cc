// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2011 Joerg Faschingbauer

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

#include "work-queue-suite.h"

#include <jf/linuxtools/joinable-thread.h>
#include <jf/linuxtools/work-queue.h>

#include <jf/unittest/test_case.h>

#include <cassert>

namespace {

using namespace jf::linuxtools;

/** Execute work asynchronously.

    For this we do not need a separate thread, as the flow is only
    one-way and no deadlock can occur. Hook the queues server-end into
    a dispatcher, post work to the queue, and spin the dispatcher
    until the work is done.
 */
class ExecuteAsyncTest : public jf::unittest::TestCase
{
public:
    ExecuteAsyncTest()
    : jf::unittest::TestCase("ExecuteAsync"),
      queue_(5) {}

    virtual void setup()
    {
        queue_.activate_object(&dispatcher_);
    }
    virtual void teardown()
    {
        queue_.deactivate_object(&dispatcher_);
    }
    virtual void run()
    {
        class SetDone : public Work
        {
        public:
            SetDone(bool* done) : done_(done) {}
            virtual void execute() { *done_ = true; }
        private:
            bool* done_;
        };
        
        bool done = false;
        queue_.execute_work_async(std::auto_ptr<Work>(new SetDone(&done)));
        while (!done)
            dispatcher_.dispatch();
    }

private:
    Dispatcher dispatcher_;
    WorkQueue queue_;
};

/** Execute work synchronously.

    Need two threads for this because the submitter has to wait, while
    the receiver works.
 */
class ExecuteSyncTest : public jf::unittest::TestCase
{
public:
    ExecuteSyncTest() : jf::unittest::TestCase("ExecuteSync") {}

    virtual void run()
    {
        class MyWorker : public JoinableThreadStarter::Worker
        {
        public:
            MyWorker(WorkQueue& queue) : queue_(queue), quit_(false) {}
            void quit() { quit_ = true; }
            virtual void run()
            {
                Dispatcher dispatcher;
                queue_.activate_object(&dispatcher);
                while (!quit_)
                    dispatcher.dispatch();
                queue_.deactivate_object(&dispatcher);
            }
        private:
            WorkQueue& queue_;
            bool quit_;
        };

        class MyRealWork : public Work
        {
        public:
            MyRealWork() : executed_(false) {}
            bool executed() const { return executed_; }
            virtual void execute() { executed_ = true; }
        private:
            bool executed_;
        };

        class MyQuitWork : public Work
        {
        public:
            MyQuitWork(MyWorker* worker) : worker_(worker) {}
            virtual void execute() { worker_->quit(); }
        private:
            MyWorker* worker_;
        };


        WorkQueue queue(5);

        MyWorker worker(queue);
        JoinableThreadStarter thread(&worker);
        thread.start();
        
        MyRealWork work;
        queue.execute_work_sync(work);
        JFUNIT_ASSERT(work.executed());

        MyQuitWork quit(&worker);
        queue.execute_work_sync(quit);
        thread.join();
    }
};

}

namespace jf {
namespace linuxtools {

WorkQueueSuite::WorkQueueSuite()
: jf::unittest::TestSuite("WorkQueue")
{
    add_test(new ExecuteAsyncTest);
    add_test(new ExecuteSyncTest);
}

}
}
