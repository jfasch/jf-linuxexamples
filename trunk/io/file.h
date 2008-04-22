// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008 Joerg Faschingbauer

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

#ifndef HAVE_JF_LINUX_IO_FILE_H
#define HAVE_JF_LINUX_IO_FILE_H

#include "io.h"

// include to get the mode flags for our users
#include <fcntl.h>

#include <string>

namespace jflinux {
namespace io {

class File : public IO
{
public:
    void create(const std::string& pathname, mode_t mode) { create(pathname.c_str(),  mode); }
    void create(const char* pathname, mode_t);

    void open(const std::string& pathname) { open(pathname.c_str()); }
    void open(const char* pathname);
};
    
}
}

#endif
