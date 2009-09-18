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
#include "pthread_suite.h"

#include "basic_thread_test.h"
#include "timed_condition_test.h"
#include "tsd_suite.h"

namespace jflinux {

PThreadSuite::PThreadSuite()
: jf::unittest::TestSuite("PThreadSuite")
{
    add_test(new BasicThreadTest);
    add_test(new TimedConditionTest);
    add_test(new ThreadSpecificDataSuite);
}

}
