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

#include <jf/linuxtools/tests/common_suite.h>
#include <jf/linuxtools/tests/pthread_suite.h>
#include <jf/linuxtools/tests/io_suite.h>
#include <jf/linuxtools/tests/events_suite.h>
#include <jf/linuxtools/tests/debug_suite.h>
#include <jf/linuxtools/tests/net_suite.h>

// CONFIX:REQUIRE_H('jf/linuxtools/tests/common_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/pthread_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/io_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/events_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/debug_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jf/linuxtools/tests/net_suite.h', REQUIRED)

namespace jf {
namespace linuxtools {

Suite::Suite()
: jf::unittest::TestSuite("LinuxTools")
{
    add_test(new CommonSuite);
    add_test(new PThreadSuite);
    add_test(new IOSuite);
    add_test(new EventsSuite);
    add_test(new DebugSuite);
    add_test(new NetSuite);
}

}
}
