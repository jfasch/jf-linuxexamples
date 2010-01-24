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

#include "file_suite.h"

#include <jflinux/file.h>
#include <jflinux/fileutil.h>

#include <jf/unittest/test_case.h>

#include <sstream>

namespace jflinux {

class FileCreateOpenTest : public jf::unittest::TestCase
{
public:
    FileCreateOpenTest() : jf::unittest::TestCase("FileCreateOpen") {}

    virtual void run()
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
                    f.create(filename, S_IRUSR|S_IWUSR);
                }
                catch (ErrnoException&) {
                    JFUNIT_FAIL();
                }
                JFUNIT_ASSERT(jflinux::FileUtil::exist(filename));
            }

            {
                File f;
                try {
                    f.open(filename, O_RDWR);
                    JFUNIT_ASSERT(f.write("x", 1) == 1);
                }
                catch (ErrnoException&) {
                    JFUNIT_FAIL();
                }
            }

            JFUNIT_ASSERT(jflinux::FileUtil::unlink(filename));
        }
    }
};

FileSuite::FileSuite()
: jf::unittest::TestSuite("File")
{
    add_test(new FileCreateOpenTest);
}

}
