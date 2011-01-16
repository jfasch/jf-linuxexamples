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
#include "fd_test.h"

#include <jf/linuxtools/fd.h>
#include <jf/linuxtools/socketpair.h>

#include <iostream>
using namespace std;

namespace jf {
namespace linuxtools {

FDTest::FDTest()
: jf::unittest::TestCase("FD") {}

void FDTest::run()
{
    // copy with refcount semantics.
    {
        FD fd2(666);
        JFUNIT_ASSERT(fd2.fd() == 666);
        FD fd3 = fd2;
        JFUNIT_ASSERT(fd3.fd() == 666);
        FD fd4(fd2);
        JFUNIT_ASSERT(fd4.fd() == 666);
        fd2 = FD();
        JFUNIT_ASSERT(!fd2.good());
        JFUNIT_ASSERT(fd3.fd() == fd4.fd() && fd4.fd() == 666);
        fd2 = fd4;
        JFUNIT_ASSERT(fd2.fd() == 666);
    }

    // explicit assignment of a file descriptor.
    {
        FD fd;
        fd.set_fd(666);
        JFUNIT_ASSERT(fd.fd() == 666);
    }

    // see if real file descriptor operations are ok.
    {
        FD left, right;
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
        left = FD();
        nread = right.read(&c, 1);
        JFUNIT_ASSERT(nread == 0);
    }
}

}
}
