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

#ifndef HAVE_JF_LINUXTOOLS_SOCKADDR_UN_H
#define HAVE_JF_LINUXTOOLS_SOCKADDR_UN_H

#include <exception>
#include <sys/socket.h>
#include <sys/un.h>

namespace jf {
namespace linuxtools {

class SockAddrUN : public sockaddr_un {
public:
    class PathTooLong : public std::exception
    {
    public:
        ~PathTooLong() throw() {}
        virtual const char* what() const throw() { return "Path too long"; }
    };
    
public:
    explicit SockAddrUN(const char* path);
};

}
}

#endif
