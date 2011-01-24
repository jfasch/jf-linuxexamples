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

#include "tcp-port.h"

#include <string.h>

namespace jf {
namespace linuxtools {

static int create_socket()
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        assert(!"error handling");
    return fd;
}

static void bind_port(int fd, const IPAddress& address, uint16_t port)
{
    struct sockaddr_in addr;
    ::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = address;

    if (::bind(fd, (const sockaddr*)&addr, sizeof(addr)) < 0)
        assert(!"error handling");
}

static uint16_t local_port_number(int fd)
{
    sockaddr_in addr;
    size_t size = sizeof(addr);
    ::memset(&addr, 0, size);
    if (::getsockname(fd, (sockaddr*)&addr, &size) < 0)
        assert(!"error handling");
    assert(size==sizeof(sockaddr_in)); // OS paranoia
    return ntohs(addr.sin_port);
}

TCPPort::TCPPort()
: port_(0)
{
    this->set_fd(create_socket());
    bind_port(this->fd(), IPAddress::any, 0);

    // we request dynamic port assignment. retrieve the port number
    // that we got assigned.
    port_ = local_port_number(this->fd());
}

TCPPort::TCPPort(const IPAddress& address)
: port_(0)
{
    this->set_fd(create_socket());
    bind_port(this->fd(), address, 0);

    // we request dynamic port assignment. retrieve the port number
    // that we got assigned.
    port_ = local_port_number(this->fd());
}

TCPPort::TCPPort(uint16_t port)
: port_(port)
{
    this->set_fd(create_socket());
    bind_port(this->fd(), IPAddress::any, port);
}

void TCPPort::listen()
{
    if (::listen(this->fd(), SOMAXCONN) < 0)
        assert(!"error handling");
}

uint16_t TCPPort::port() const
{
    assert(port_!=0);
    return port_;
}

TCPEndpoint TCPPort::accept()
{
    int newfd = ::accept(this->fd(), NULL, 0);
    assert(newfd>=0); // error handling
    return TCPEndpoint(newfd);
}

}
}
