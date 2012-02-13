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

#include "suite.h"

#include <jf/linuxtools/tests/common-suite.h>
#include <jf/linuxtools/tests/pthread-suite.h>
#include <jf/linuxtools/tests/io-suite.h>
#include <jf/linuxtools/tests/events-suite.h>
#include <jf/linuxtools/tests/linux-events-suite.h>
#include <jf/linuxtools/tests/net-suite.h>
#include <jf/linuxtools/tests/posix-ipc-suite.h>
#include <jf/linuxtools/tests/debug-suite.h>

// CONFIX:REQUIRE_H('jf/linuxtools/tests/common-suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/pthread-suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/io-suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/events-suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/linux-events-suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/net-suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/posix-ipc-suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/debug-suite.h', REQUIRED)

namespace jf {
namespace linuxtools {

Suite::Suite()
: jf::unittest::TestSuite("LinuxTools")
{
    add_test(std::auto_ptr<Test>(new CommonSuite));
    add_test(std::auto_ptr<Test>(new PthreadSuite));
    add_test(std::auto_ptr<Test>(new IOSuite));
    add_test(std::auto_ptr<Test>(new EventsSuite));
    add_test(std::auto_ptr<Test>(new LinuxEventsSuite));
    add_test(std::auto_ptr<Test>(new NetSuite));
    add_test(std::auto_ptr<Test>(new PosixIPCSuite));
    add_test(std::auto_ptr<Test>(new DebugSuite));
}

}
}
