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

#include "file_test.h"

#include <jflinux/io/file.h>

#include <sstream>

namespace jflinux {
namespace io {
namespace tests {

FileTest::FileTest()
: jf::unittest::TestCase("jflinux::io::tests::FileTest") {}

void FileTest::run()
{
    std::string pid_str;
    {
        std::ostringstream tmp;
        tmp << getpid();
        pid_str = tmp.str();
    }
    
    {
        std::string filename("/tmp/");
        filename += pid_str;
        filename += "_0";

        {
            File f;
            try {
                f.create(filename);
            }
            catch (IO::Exception&) {
                JFUNIT_FAIL();
            }
            JFUNIT_ASSERT(jflinux::FileUtil::exist(filename));
        }

        {
            File f;
            try {
                f.open(filename);
                JFUNIT_ASSERT(f.write("x", 1) == 1);
            }
            catch (IO::Exception&) {
                JFUNIT_FAIL();
            }
        }

        JFUNIT_ASSERT(jflinux::FileUtil::unlink(filename));
    }
}

}
}
}
