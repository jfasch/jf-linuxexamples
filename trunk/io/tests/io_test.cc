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
#include "io_test.h"

#include <jflinux/io.h>
#include <jflinux/socketpair.h>

#include <iostream>
using namespace std;

namespace jflinux {

IOTest::IOTest()
: jf::unittest::TestCase("IOTest") {}

void IOTest::run()
{
    // copy with refcount semantics.
    {
        IO io1;
        JFUNIT_ASSERT_THROWS(ErrnoException, io1.fd());
        IO io2(666);
        JFUNIT_ASSERT(io2.fd() == 666);
        IO io3 = io2;
        JFUNIT_ASSERT(io3.fd() == 666);
        IO io4(io2);
        JFUNIT_ASSERT(io4.fd() == 666);
        io2 = IO();
        JFUNIT_ASSERT_THROWS(ErrnoException, io2.fd());
        JFUNIT_ASSERT(io3.fd() == io4.fd() && io4.fd() == 666);
        io2 = io4;
        JFUNIT_ASSERT(io2.fd() == 666);
    }

    // operations on bad objects must fail.
    {
        IO io;
        JFUNIT_ASSERT(!io.good());
        JFUNIT_ASSERT_THROWS(ErrnoException, io.write("1", 1));
        char c;
        JFUNIT_ASSERT_THROWS(ErrnoException, io.read(&c, 1));
    }

    // explicit assignment of a file descriptor.
    {
        IO io;
        io.set_fd(666);
        JFUNIT_ASSERT(io.fd() == 666);
    }

    // see if real file descriptor operations are ok.
    {
        IO left, right;
        {
            SocketPair pair;
            left = pair.left();
            right = pair.right();
            // pair goes out of scope => left and right are the only
            // references to our file descriptors.
        }
        
        JFUNIT_ASSERT(left.good());
        JFUNIT_ASSERT(right.good());
        int nwritten = left.write("1", 1);
        JFUNIT_ASSERT(nwritten == 1);
        char c;
        int nread = right.read(&c, 1);
        JFUNIT_ASSERT(nread == 1);

        // since left and right are the only references to our
        // sockets, the following must close left. thus, reading from
        // right must signal us an end-of-file.
        left = IO();
        nread = right.read(&c, 1);
        JFUNIT_ASSERT(nread == 0);
    }
}

}
