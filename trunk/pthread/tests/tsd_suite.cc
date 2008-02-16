// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008 Joerg Faschingbauer

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

#include "tsd_suite.h"

#include <jflinux/pthread/thread_specific.h>
#include <jflinux/pthread/joinable_thread.h>
#include <jflinux/pthread/future.h>

#include <jf/unittest/test_case.h>

namespace {

class SeparateValuesWorker : public jflinux::pthread::JoinableThreadStarter::Worker
{
public:
    static void tsd_dtor(void* data) { delete (int*)data; }
    static jflinux::pthread::ThreadSpecific<int> the_semi_global_thing;

public:
    SeparateValuesWorker(jflinux::pthread::Future<bool>* retval) : retval_(retval) {}
    virtual void run()
    {
        // our context has not yet touched the TSD, so we must not see
        // a value.
        if (the_semi_global_thing.get() != NULL) {
            retval_->set(false);
            return;
        }

        // set the value.
        the_semi_global_thing.set(new int(2));
        retval_->set(true);
    }

private:
    jflinux::pthread::Future<bool>* retval_;
};
jflinux::pthread::ThreadSpecific<int> SeparateValuesWorker::the_semi_global_thing(SeparateValuesWorker::tsd_dtor);

class DestructorWorker : public jflinux::pthread::JoinableThreadStarter::Worker
{
public:
    static jflinux::pthread::Future<bool> destroyed;
    static void tsd_dtor(void* data) 
    {
        delete (int*)data;
        destroyed.set(true);
    }
    static jflinux::pthread::ThreadSpecific<int> the_semi_global_thing;

public:
    virtual void run()
    {
        the_semi_global_thing.set(new int);
    }
};
jflinux::pthread::Future<bool> DestructorWorker::destroyed;
jflinux::pthread::ThreadSpecific<int> DestructorWorker::the_semi_global_thing(DestructorWorker::tsd_dtor);

} // /<anonymous>

namespace jflinux {
namespace pthread {
namespace tests {

// the main thread and a dedicated worker thread make use of the same
// TSD slot. both check that they see their own values (ok, this is
// more of a comprehension test).
class SeparateValuesTest : public jf::unittest::TestCase
{
public:
    SeparateValuesTest() : jf::unittest::TestCase("jflinux::pthread::tests::SeparateValuesTest") {}
    virtual void run()
    {
        SeparateValuesWorker::the_semi_global_thing.set(new int(1));
        
        jflinux::pthread::Future<bool> result;
        jflinux::pthread::JoinableThreadStarter starter(new SeparateValuesWorker(&result));
        starter.start();

        // our slave thread hasn't seen anything particularly bad.
        JFUNIT_ASSERT(result.get() == true);

        // see if nobody has messed with our TSD.
        JFUNIT_ASSERT(SeparateValuesWorker::the_semi_global_thing.get());
        JFUNIT_ASSERT(*SeparateValuesWorker::the_semi_global_thing.get() == 1);
    }
};

// spawn a worker thread whose sole purpose it is to create a TSD
// value. a special dtor function is used to communicate to the main
// test code that the dtor has been called.
class DestructorTest : public jf::unittest::TestCase
{
public:
    DestructorTest() : jf::unittest::TestCase("jflinux::pthread::tests::DestructorTest") {}
    virtual void run()
    {
        DestructorWorker::the_semi_global_thing.set(new int(1));
        
        jflinux::pthread::JoinableThreadStarter starter(new DestructorWorker);
        starter.start();

        // synchronize with the dtor call. NOTE thaht we cannot simply
        // check a boolean value without synchronization - this value
        // is not guaranteed to be set at this point.
        DestructorWorker::destroyed.get();
    }
};

ThreadSpecificDataSuite::ThreadSpecificDataSuite()
: jf::unittest::TestSuite("jflinux::pthread::tests::ThreadSpecificDataTest")
{
    add_test(new SeparateValuesTest);
    add_test(new DestructorTest);
}

}
}
}