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

#include "tcp-suite.h"

#include <jf/linuxtools/tcp-port.h>
#include <jf/linuxtools/tcp-connect.h>

#include <jf/unittest/test_case.h>

namespace {

using namespace jf::linuxtools;

class PortLocalAddressDynamicPort : public jf::unittest::TestCase
{
public:
    PortLocalAddressDynamicPort() : jf::unittest::TestCase("PortLocalAddressDynamicPort") {}
    virtual void run()
    {
        TCPPort port(IPAddress("127.0.0.1"));
        port.listen();
        int port_number = port.port();

        TCPEndpoint client_connection = tcp_connect(IPAddress("127.0.0.1"), port_number);
        TCPEndpoint server_connection = port.accept();

        JFUNIT_ASSERT(client_connection.write("", 1) == 1);
        char c;
        JFUNIT_ASSERT(server_connection.read(&c, 1) == 1);

        client_connection.close();
        JFUNIT_ASSERT(server_connection.read(&c, 1) == 0);
    }
};

class PortAnyAddressDynamicPort : public jf::unittest::TestCase
{
public:
    PortAnyAddressDynamicPort() : jf::unittest::TestCase("PortAnyAddressDynamicPort") {}
    virtual void run()
    {
        TCPPort port;
        port.listen();
        int port_number = port.port();

        TCPEndpoint client_connection = tcp_connect(IPAddress("127.0.0.1"), port_number);
        TCPEndpoint server_connection = port.accept();

        JFUNIT_ASSERT(client_connection.write("", 1) == 1);
        char c;
        JFUNIT_ASSERT(server_connection.read(&c, 1) == 1);

        client_connection.close();
        JFUNIT_ASSERT(server_connection.read(&c, 1) == 0);
    }
};

class PortAnyAddressFixedPort : public jf::unittest::TestCase
{
public:
    PortAnyAddressFixedPort() : jf::unittest::TestCase("PortAnyAddressFixedPort") {}
    virtual void run()
    {
        for (uint16_t port_number = 1024; port_number<65536; port_number++) {
            std::auto_ptr<TCPPort> port;
            try {
                port.reset(new TCPPort(port_number));
            }
            catch (const ErrnoException& e) {
                if (e.error() == EADDRINUSE)
                    continue;
                else
                    JFUNIT_FAIL();
            }

            port->listen();
            uint16_t port_number = port->port();

            TCPEndpoint client_connection = tcp_connect(IPAddress("127.0.0.1"), port_number);
            TCPEndpoint server_connection = port->accept();

            JFUNIT_ASSERT(client_connection.write("", 1) == 1);
            char c;
            JFUNIT_ASSERT(server_connection.read(&c, 1) == 1);

            client_connection.close();
            JFUNIT_ASSERT(server_connection.read(&c, 1) == 0);

            return;
        }
    }
};

}

namespace jf {
namespace linuxtools {

TCPSuite::TCPSuite()
: jf::unittest::TestSuite("TCPSuite")
{
    add_test(std::auto_ptr<Test>(new PortLocalAddressDynamicPort));
    add_test(std::auto_ptr<Test>(new PortAnyAddressDynamicPort));
    add_test(std::auto_ptr<Test>(new PortAnyAddressFixedPort));
}

}
}
