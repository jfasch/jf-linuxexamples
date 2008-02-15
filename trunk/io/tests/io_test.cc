// (C) Atronic -*- C++ -*-

#include "io_io_refcounting_test.h"

#include "io_io.h"
#include "io_socket.h"
#include "io_socketpair.h"

namespace io
{

    IORefCountingTest::IORefCountingTest()
    : kunit::TestCase("io::IORefCountingTest") {}

    void IORefCountingTest::RunTest()
    {
        // copy with refcount semantics.
        {
            IO io1;
            KUNIT_ASSERT(io1.GetFD() == -1);
            IO io2(666);
            KUNIT_ASSERT(io2.GetFD() == 666);
            IO io3 = io2;
            KUNIT_ASSERT(io3.GetFD() == 666);
            IO io4(io2);
            KUNIT_ASSERT(io4.GetFD() == 666);
            io2 = IO();
            KUNIT_ASSERT(io2.GetFD() == -1);
            KUNIT_ASSERT(io3.GetFD() == io4.GetFD() && io4.GetFD() == 666);
            io2 = io4;
            KUNIT_ASSERT(io2.GetFD() == 666);
        }

        // operations on bad objects must fail.
        {
            IO io;
            KUNIT_ASSERT(!io.Good());
            KUNIT_ASSERT(io.Write("1", 1) == -1);
            char c;
            KUNIT_ASSERT(io.Read(&c, 1) == -1);
        }

        // explicit assignment of a file descriptor.
        {
            IO io;
            io.SetFD(666);
            KUNIT_ASSERT(io.GetFD() == 666);
        }

        // see if real file descriptor operations are ok.
        {
            Socket left, right;
            {
                SocketPair pair;
                left = pair.Left();
                right = pair.Right();
                // pair goes out of scope => left and right are the only
                // references to our file descriptors.
            }
        
            KUNIT_ASSERT(left.Good());
            KUNIT_ASSERT(right.Good());
            int nwritten = left.Write("1", 1);
            KUNIT_ASSERT(nwritten == 1);
            char c;
            int nread = right.Read(&c, 1);
            KUNIT_ASSERT(nread == 1);

            // since left and right are the only references to our
            // sockets, the following must close left. thus, reading from
            // right must signal us an end-of-file.
            left = Socket();
            nread = right.Read(&c, 1);
            KUNIT_ASSERT(nread == 0);
        }
    }

};
