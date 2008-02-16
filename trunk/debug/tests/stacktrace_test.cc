// -*- C++ -*-

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

#include <jflinux/debug/stacktrace.h>

// * f0
//   * f00
//     * f000
//   * f01
//     * f010

namespace {

using namespace jf::debug;

void f000() {
    JFSTACKTRACE();
}

void f010() {
    JFSTACKTRACE();
}

void f00() {
    JFSTACKTRACE();
    f000();
}

void f01() {
    JFSTACKTRACE();
    f010();
}

void f0() {
    JFSTACKTRACE();
    f00();
    f01();
}

} // /<anonymous>

namespace jflinux {
namespace debug {
namespace tests {

void StackTraceTest::run()
{

    StackHistory history;
    StackHistory::set_current(&history);
    f0();

    JFUNIT_ASSERT(history.descendants().size() == 1);
    const StackFrame& s0 = history.descendants()[0];
    JFUNIT_ASSERT(s0.data() == f0);

    JFUNIT_ASSERT(s0.descendants().size() == 2);
    const StackFrame& s00 = s0.descendants()[0];
    JFUNIT_ASSERT(s00.data() == f00);
    const StackFrame& s01 = s0.descendants()[0];
    JFUNIT_ASSERT(s01.data() == f01);
    
    JFUNIT_ASSERT(s00.descendants().size() == 1);
    const StackFrame& s000 = s00.descendants()[0];
    JFUNIT_ASSERT(s000.data() == f000);

    JFUNIT_ASSERT(s01.descendants().size() == 1);
    const StackFrame& s010 = s01.descendants()[0];
    JFUNIT_ASSERT(s010.data() == f010);
}

}
}
}
