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

#include "shm-suite.h"

#include <jf/linuxtools/shm.h>

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
class BasicTest : public jf::unittest::TestCase
{
public:
    BasicTest() : jf::unittest::TestCase("Basic") {}
    virtual void teardown()
    {
        SHM::unlink(shm_name_.c_str());
    }
    virtual void run()
    {
        char tmp[128];
        sprintf(tmp, "/SHMSuite-Basic-%ld", random());
        shm_name_ = tmp;

        SHM shm1 = SHM::create(shm_name_.c_str(), O_RDWR, 0600, 1024);
        void* shm_region_1 = shm1.map(1024, PROT_WRITE|PROT_READ, MAP_SHARED, 0);

        SHM shm2 = SHM::open(shm_name_.c_str(), O_RDONLY);
        void* shm_region_2 = shm2.map(1024, PROT_READ, MAP_SHARED, 0);

        *(((char*)shm_region_1)+2) = 'a';
        JFUNIT_ASSERT(*(((char*)shm_region_2)+2) == 'a');
    }

private:
    std::string shm_name_;
};

// ------------------------------------------------------------
class UnrelatedProcessesUsingSameSHM : public jf::unittest::TestCase
{
public:
    UnrelatedProcessesUsingSameSHM() : jf::unittest::TestCase("UnrelatedProcessesUsingSameSHM") {}
    virtual void teardown()
    {
        SHM::unlink(shm_name_.c_str());
    }
    virtual void run()
    {
        char tmp[128];
        sprintf(tmp, "/SHMSuite-Basic-%ld", random());
        shm_name_ = tmp;

        SHM shm = SHM::create(shm_name_.c_str(), O_RDWR, 0600, 1024);
        shm = SHM();

        pid_t setter = fork();
        if (setter == 0) { // child
            SHM set_shm = SHM::open(shm_name_.c_str(), O_RDWR);
            void* memory;
            try { memory = set_shm.map(1024, PROT_WRITE, MAP_SHARED, 0); }
            catch (const std::exception&) { exit(1); }
            *(((char*)memory)+2) = 'a';
            exit(0);
        }

        int setter_status;
        JFUNIT_ASSERT(waitpid(setter, &setter_status, 0) == setter);
        JFUNIT_ASSERT(WIFEXITED(setter_status));
        JFUNIT_ASSERT(WEXITSTATUS(setter_status) == 0);

        pid_t getter = fork();
        if (getter == 0) { // child
            SHM get_shm = SHM::open(shm_name_.c_str(), O_RDONLY);
            void* memory;
            try { memory = get_shm.map(1024, PROT_READ, MAP_SHARED, 0); }
            catch (const std::exception&) { exit(1); }
            if (*(((char*)memory)+2) == 'a')
                exit(0);
            else
                exit(1);
        }

        int getter_status;
        JFUNIT_ASSERT(waitpid(getter, &getter_status, 0) == getter);
        JFUNIT_ASSERT(WIFEXITED(getter_status));
        JFUNIT_ASSERT(WEXITSTATUS(getter_status) == 0);
    }

private:
    std::string shm_name_;
};

}

namespace jf {
namespace linuxtools {

SHMSuite::SHMSuite()
: jf::unittest::TestSuite("SHM")
{
    add_test(new BasicTest);
    add_test(new UnrelatedProcessesUsingSameSHM);
}

}
}
