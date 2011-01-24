// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2011 Joerg Faschingbauer

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

#include "basic-thread-test.h"

#include <jf/linuxtools/joinable-thread.h>

namespace {

class TestWorker : public jf::linuxtools::JoinableThreadStarter::Worker {
public:
    TestWorker() : was_running_(false) {}
    bool was_running() const { return was_running_; }

    virtual void run() {
        was_running_ = true;
    }

private:
    bool was_running_;
};

}

namespace jf {
namespace linuxtools {

void BasicThreadTest::run()
{
    TestWorker worker;
    JoinableThreadStarter t(&worker);
    t.start();
    t.join();
    JFUNIT_ASSERT(worker.was_running());
}

}
}
