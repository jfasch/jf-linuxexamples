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

#ifndef HAVE_JF_LINUX_FILEUTIL_H
#define HAVE_JF_LINUX_FILEUTIL_H

#include <string>

namespace jflinux {

class FileUtil
{
public:
    static bool exist(const char* pathname);
    static bool exist(const std::string& pathname) { return exist(pathname.c_str()); }

    static bool unlink(const char* pathname);
    static bool unlink(const std::string& pathname) { return exist(pathname.c_str()); }
};

}

#endif
