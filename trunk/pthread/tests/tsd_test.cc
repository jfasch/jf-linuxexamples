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

#include "tsd_test.h"

#include <jflinux/thread_specific.h>
#include <jflinux/joinable_thread.h>

namespace {

jflinux::ThreadSpecific<int> the_semi_global_thing;

}

class Worker : public jflinux::JoinableThreadStarter::Worker
{
public:
    virtual void run()
    {
        
    }
};

namespace jflinux {
namespace tests {

void ThreadSpecificDataTest::run()
{
    JFUNIT_FAIL();
}

}
}
