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

#include "sem-suite.h"

#include <jf/linuxtools/sem.h>

#include <jf/unittest/test_case.h>

#include <stdio.h>
#include <sys/wait.h>

namespace {

class RandomInit
{
public:
    RandomInit()
    {
        srandom(time(NULL));
    }
};
static RandomInit randominit;


using namespace jf::linuxtools;

// ------------------------------------------------------------
class SemaphoreTest : public jf::unittest::TestCase
{
public:
    SemaphoreTest() : jf::unittest::TestCase("Semaphore") {}
    virtual void teardown()
    {
        SEM::unlink(sem_kickright_name_.c_str());
        SEM::unlink(sem_kickleft_name_.c_str());
    }
    virtual void run()
    {
        char tmp[128];
        long num = random();
        sprintf(tmp, "/SEMSuite-SemaphoreTest-%ld-kickright", num);
        sem_kickright_name_ = tmp;
        sprintf(tmp, "/SEMSuite-SemaphoreTest-%ld-kickleft", num);
        sem_kickleft_name_ = tmp;

        // create two semaphores, and close them immediately
        // afterwards. they're only for the two children's use.
        {
            SEM sem_kickright = SEM::create(sem_kickright_name_.c_str(), O_RDWR, 0600, 0);
            SEM sem_kickleft = SEM::create(sem_kickleft_name_.c_str(), O_RDWR, 0600, 0);
        }

        // child "left" kicks right, and then waits to be kicked.
        pid_t left = fork();
        if (left == 0) { // child
            SEM sem_kickright = SEM::open(sem_kickright_name_.c_str(), O_RDWR);
            SEM sem_kickleft = SEM::open(sem_kickleft_name_.c_str(), O_RDONLY /* we only wait */);
            sem_kickright.post();
            sem_kickleft.wait();
            exit(0);
        }

        // child "right" waits to be kicked, and then kicks left.
        pid_t right = fork();
        if (right == 0) { // child
            SEM sem_kickright = SEM::open(sem_kickright_name_.c_str(), O_RDONLY /* we only wait */);
            SEM sem_kickleft = SEM::open(sem_kickleft_name_.c_str(), O_RDWR);
            sem_kickright.wait();
            sem_kickleft.post();
            exit(0);
        }

        int left_status;
        JFUNIT_ASSERT(waitpid(left, &left_status, 0) == left);
        JFUNIT_ASSERT(WIFEXITED(left_status));
        JFUNIT_ASSERT(WEXITSTATUS(left_status) == 0);
        
        int right_status;
        JFUNIT_ASSERT(waitpid(right, &right_status, 0) == right);
        JFUNIT_ASSERT(WIFEXITED(right_status));
        JFUNIT_ASSERT(WEXITSTATUS(right_status) == 0);
    }

private:
    std::string sem_kickright_name_;
    std::string sem_kickleft_name_;
};

}

namespace jf {
namespace linuxtools {

SEMSuite::SEMSuite()
: jf::unittest::TestSuite("Semaphore")
{
    add_test(new SemaphoreTest);
}

}
}
