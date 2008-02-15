// (C) Atronic -*- C++ -*-

#include "io_test.h"

#include <jflinux/io/io.h>
#include <jflinux/io/socketpair.h>

namespace jflinux {
namespace io {
namespace tests {

IOTest::IOTest()
: jf::unittest::TestCase("jflinux::io::tests::IOTest") {}

void IOTest::run()
{
    // copy with refcount semantics.
    {
        IO io1;
        JFUNIT_ASSERT(io1.fd() == -1);
        IO io2(666);
        JFUNIT_ASSERT(io2.fd() == 666);
        IO io3 = io2;
        JFUNIT_ASSERT(io3.fd() == 666);
        IO io4(io2);
        JFUNIT_ASSERT(io4.fd() == 666);
        io2 = IO();
        JFUNIT_ASSERT(io2.fd() == -1);
        JFUNIT_ASSERT(io3.fd() == io4.fd() && io4.fd() == 666);
        io2 = io4;
        JFUNIT_ASSERT(io2.fd() == 666);
    }

    // operations on bad objects must fail.
    {
        IO io;
        JFUNIT_ASSERT(!io.good());
        JFUNIT_ASSERT_THROWS(IO::Exception, io.write("1", 1));
        char c;
        JFUNIT_ASSERT_THROWS(IO::Exception, io.read(&c, 1));
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
}
}
