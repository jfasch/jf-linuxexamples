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

#ifndef HAVE_JF_LINUXTOOLS_IPADDRESS_H
#define HAVE_JF_LINUXTOOLS_IPADDRESS_H

#include <arpa/inet.h>
#include <netinet/in.h>

#include <exception>
#include <string>

namespace jf {
namespace linuxtools {

class IPAddress : public in_addr {
public:
    class BadAddress : public std::exception
    {
    public:
        BadAddress(const std::string& bad_address_string);
        ~BadAddress() throw() {}
        virtual const char* what() const throw() { return message_.c_str(); }
    private:
        std::string message_;
    };
    
public:
    IPAddress();
    IPAddress(in_addr);
    /** From address in network byte order. */
    explicit IPAddress(uint32_t);
    /* Addr in numbers-and-dots. Throws badly if conversion fails. */
    explicit IPAddress(const char* hostaddr);
    explicit IPAddress(const std::string& hostaddr);

    /* Address, converted to numbers-and-dots by inet_ntoa(). */
    std::string host_string() const; // my sockaddr_in.sin_addr, converted by inet_ntoa()

    static IPAddress any;
};

}
}

#endif
