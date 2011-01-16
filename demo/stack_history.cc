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

#include <jf/linuxtools/stack_history.h>
#include <jf/linuxtools/stack_history_print.h>

#include <iostream>

using namespace jf::linuxtools;

void f0();
void f00();
void f01();
void f000();
void f010();

int main()
{
    __PRETTY_FUNCTION__StackHistory h;
    __PRETTY_FUNCTION__StackHistory::set_instance(&h);
    f0();

    print(std::cout, h);

    return 0;
}

void f0()
{
    JF_STACKTRACE();
    f00();
    f01();
}

void f00()
{
    JF_STACKTRACE();
    f000();
}

void f01()
{
    JF_STACKTRACE();
    f010();
}

void f000()
{
    JF_STACKTRACE();
}

void f010()
{
    JF_STACKTRACE();
}
