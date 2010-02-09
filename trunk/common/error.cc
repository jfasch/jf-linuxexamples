// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2010 Joerg Faschingbauer

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

#include "error.h"

#include <string.h>

namespace jf {
namespace linuxtools {

const char* ErrnoException::what() const throw()
{
    // should suffice to store error strings like "Permission denied"
    // or "No such device".
    static const size_t buflen = 256;
    char buf[256]; 

    return strerror_r(error_, buf, buflen);
}

}
}
