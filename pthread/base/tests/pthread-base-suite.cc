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

#include "pthread-base-suite.h"

#include "basic-thread-suite.h"
#include "timed-condition-test.h"
#include "tsd-suite.h"
#include "threadpool-suite.h"

namespace jf {
namespace linuxtools {

PthreadBaseSuite::PthreadBaseSuite()
: jf::unittest::TestSuite("PthreadBase")
{
    add_test(std::auto_ptr<Test>(new BasicThreadSuite));
    add_test(std::auto_ptr<Test>(new TimedConditionTest));
    add_test(std::auto_ptr<Test>(new ThreadSpecificDataSuite));
    add_test(std::auto_ptr<Test>(new ThreadPoolSuite));
}

}
}
