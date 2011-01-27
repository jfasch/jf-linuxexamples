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

#ifndef HAVE_JF_LINUXTOOLS_UNIX_PORT_H
#define HAVE_JF_LINUXTOOLS_UNIX_PORT_H

#include "unix-endpoint.h"

#include <jf/linuxtools/fd.h>
#include <jf/linuxtools/error.h>

namespace jf {
namespace linuxtools {

class UNIXPort : public FD
{
public:
    UNIXPort(const char* path);

    /** Start listening (i.e., watching out for incoming connections).

        @throws ErrnoException
    */
    void listen();

    /** Accept one connection.

        @return The accepted connection.
        @throws ErrnoException
     */
    UNIXEndpoint accept();
};

}
}

#endif
