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

#include "unix-port.h"

#include "unix-address.h" 

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

namespace jf {
namespace linuxtools {

static int create_socket()
{
    int fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
        throw ErrnoException(errno, "socket(AF_UNIX,SOCK_STREAM,0)");
    return fd;
}

static void bind_port(int fd, const char* path)
{
    SockAddrUN addr(path);
    if (bind(fd, (const sockaddr*)&addr, sizeof(addr)) < 0)
        throw ErrnoException(errno, "bind()");
}

UNIXPort::UNIXPort(const char* path)
{
    this->set_fd(create_socket());
    bind_port(this->fd(), path);
}

void UNIXPort::listen()
{
    if (::listen(this->fd(), SOMAXCONN) < 0)
        throw ErrnoException(errno, "listen()");
}

UNIXEndpoint UNIXPort::accept()
{
    int newfd = ::accept(this->fd(), NULL, 0);
    if (newfd < 0)
        throw ErrnoException(errno, "accept()");
    return UNIXEndpoint(newfd);
}

}
}
