// -*- C++ -*-

// Copyright (C) 2008-2010 Joerg Faschingbauer

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

#include "stack_history_test.h"

#include <jf/linuxtools/stack_history.h>

#include <iostream>
using namespace std;

// * f0
//   * f00
//     * f000
//   * f01
//     * f010

using namespace jf::linuxtools;

namespace {

typedef void(*vfunc)(void);
typedef jf::linuxtools::StackHistory<vfunc> TestStackHistory;
typedef jf::linuxtools::StackFrame<vfunc> TestStackFrame;
typedef jf::linuxtools::StackElement<vfunc> TestStackElement;

#define TESTSTACKTRACE(fn) TestStackFrame teststackframe(fn, TestStackHistory::instance());

void f000() {
    TESTSTACKTRACE(f000);
}

void f010() {
    TESTSTACKTRACE(f010);
}

void f00() {
    TESTSTACKTRACE(f00);
    f000();
}

void f01() {
    TESTSTACKTRACE(f01);
    f010();
}

void f0() {
    TESTSTACKTRACE(f0);
    f00();
    f01();
}

} // /<anonymous>

namespace jf {
namespace linuxtools {

void StackHistoryTest::run()
{

    TestStackHistory history;
    TestStackHistory::set_instance(&history);
    f0();

    JFUNIT_ASSERT(history.root().descendants().size() == 1);
    const TestStackElement* e0 = history.root().descendants()[0];
    JFUNIT_ASSERT(e0->description() == f0);

    JFUNIT_ASSERT(e0->descendants().size() == 2);
    const TestStackElement* e00 = e0->descendants()[0];
    JFUNIT_ASSERT(e00->description() == f00);
    const TestStackElement* e01 = e0->descendants()[1];
    JFUNIT_ASSERT(e01->description() == f01);
    
    JFUNIT_ASSERT(e00->descendants().size() == 1);
    const TestStackElement* e000 = e00->descendants()[0];
    JFUNIT_ASSERT(e000->description() == f000);

    JFUNIT_ASSERT(e01->descendants().size() == 1);
    const TestStackElement* e010 = e01->descendants()[0];
    JFUNIT_ASSERT(e010->description() == f010);
}

}
}
