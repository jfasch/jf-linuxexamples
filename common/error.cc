// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2011 Joerg Faschingbauer

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

ErrnoException::ErrnoException(int error, const char* message)
: message_(message),
  error_(error)
{
    message_ += ": ";
    char buf[64];
    message_ += ::strerror_r(error_, buf, sizeof(buf));
    ::sprintf(buf, " (%d)", error_);
    message_ += buf;
}

const char* ErrnoException::what() const throw()
{
    return message_.c_str();
}

}
}
