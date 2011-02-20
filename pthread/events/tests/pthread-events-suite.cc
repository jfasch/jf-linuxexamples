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

#include "pthread-events-suite.h"

#include "event-message-queue-suite.h"
#include "work-queue-suite.h"
#include "worker-thread-suite.h"

namespace jf {
namespace linuxtools {

PthreadEventsSuite::PthreadEventsSuite()
: jf::unittest::TestSuite("PthreadEvents")
{
    add_test(std::auto_ptr<Test>(new EventMessageQueueSuite));
    add_test(std::auto_ptr<Test>(new WorkQueueSuite));
    add_test(std::auto_ptr<Test>(new WorkerThreadSuite));
}

}
}
