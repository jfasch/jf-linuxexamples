// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2012 Joerg Faschingbauer

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

#include "basic-thread-suite.h"

#include <jf/linuxtools/joinable-thread.h>
#include <jf/unittest/case.h>

#include <cassert>

namespace {

using namespace jf::linuxtools;

// ------------------------------------------------------------------
class BasicThreadTest : public jf::unittest::TestCase
{
public:
    BasicThreadTest() : jf::unittest::TestCase("Basic") {}

    virtual void run()
    {
        class MyWorker : public JoinableThreadStarter::Worker
        {
        public:
            MyWorker() : was_running_(false) {}
            bool was_running() const { return was_running_; }
            virtual void run() { was_running_ = true; }
        private:
            bool was_running_;
        };

        MyWorker worker;
        JoinableThreadStarter t(&worker);
        t.start();
        t.join();
        JFUNIT_ASSERT(worker.was_running());
    }
};

// ------------------------------------------------------------------
class DeleteWithoutStart : public jf::unittest::TestCase
{
public:
    DeleteWithoutStart() : jf::unittest::TestCase("DeleteWithoutStart") {}

    virtual void run()
    {
        class MyWorker : public JoinableThreadStarter::Worker
        {
        public:
            virtual void run() { assert(false); }
        };

        MyWorker worker;
        JoinableThreadStarter t(&worker);
    }
};

}

namespace jf {
namespace linuxtools {

BasicThreadSuite::BasicThreadSuite()
: jf::unittest::TestSuite("Basic")
{
    add_test(std::auto_ptr<Test>(new BasicThreadTest));
    add_test(std::auto_ptr<Test>(new DeleteWithoutStart));
}

}
}
