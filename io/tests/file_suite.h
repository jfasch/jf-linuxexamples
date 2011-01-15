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
#ifndef HAVE_JF_LINUXTOOLS_FILE_SUITE_H
#define HAVE_JF_LINUXTOOLS_FILE_SUITE_H

#include <jf/unittest/test_suite.h>

namespace jf {
namespace linuxtools {

class FileSuite : public jf::unittest::TestSuite
{
public:
    FileSuite();
};

}
}

#endif
