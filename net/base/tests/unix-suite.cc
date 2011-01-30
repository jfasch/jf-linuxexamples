// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2011 Joerg Faschingbauer

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

#include "unix-suite.h"

#include <jf/linuxtools/unix-port.h>
#include <jf/linuxtools/unix-connect.h>
#include <jf/linuxtools/unix-address.h>

#include <jf/unittest/test_case.h>

#include <unistd.h>
#include <stdio.h>

namespace {

using namespace jf::linuxtools;

class BasicTest : public jf::unittest::TestCase
{
public:
    BasicTest() : jf::unittest::TestCase("Basic") {}
    virtual void teardown()
    {
        unlink(socketname_.c_str());
    }
    virtual void run()
    {
        char prefix[64];
        sprintf(prefix, "UNIXDomain-BasicTest-%d", getpid());
        socketname_ = tempnam("/tmp", prefix);
        
        UNIXPort port(socketname_);
        port.listen();

        UNIXEndpoint client_connection = unix_connect(socketname_);
        UNIXEndpoint server_connection = port.accept();

        JFUNIT_ASSERT(client_connection.write("", 1) == 1);
        char c;
        JFUNIT_ASSERT(server_connection.read(&c, 1) == 1);

        client_connection.close();
        JFUNIT_ASSERT(server_connection.read(&c, 1) == 0);
    }
private:
    std::string socketname_;
};

class BadPathTest : public jf::unittest::TestCase
{
public:
    BadPathTest() : jf::unittest::TestCase("BadPath") {}
    virtual void run()
    {
        // chances are this doesn't exist
        JFUNIT_ASSERT_THROWS(ErrnoException, UNIXPort("/sbcsds/sdcuapasggs/dhgjuijiuh/cgtjhfn"));
    }
};

class PathTooLongTest : public jf::unittest::TestCase
{
public:
    PathTooLongTest() : jf::unittest::TestCase("PathTooLong") {}
    virtual void run()
    {
        sockaddr_un addr;
        char tmp[sizeof(addr.sun_path) + 1];
        memset(tmp, 'a', sizeof(addr.sun_path));
        tmp[sizeof(addr.sun_path)] = 0;
        JFUNIT_ASSERT_THROWS(SockAddrUN::PathTooLong, SockAddrUN x(tmp));
    }
};

}

namespace jf {
namespace linuxtools {

UNIXSuite::UNIXSuite()
: jf::unittest::TestSuite("UNIX-Domain")
{
    add_test(new BasicTest);
    add_test(new BadPathTest);
    add_test(new PathTooLongTest);
}

}
}
