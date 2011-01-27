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

#include "tcp-connect.h"

namespace jf {
namespace linuxtools {

TCPEndpoint tcp_connect(const IPAddress& address, uint16_t port)
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw ErrnoException(errno, "socket()");

    TCPEndpoint endpoint(fd);

    sockaddr_in addr;
    addr.sin_addr = address;
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    if (::connect(endpoint.fd(), (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw ErrnoException(errno, "connect()");

    return endpoint;
}

}
}
