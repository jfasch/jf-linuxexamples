// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2011-2012 Joerg Faschingbauer

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

#include "mq-suite.h"

#include <jf/linuxtools/mq.h>

#include <jf/unittest/case.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
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
        MQ::unlink(mq_name_);
    }
    virtual void run()
    {
        char tmp[128];
        sprintf(tmp, "/MQSuite-Basic-%ld", random());
        mq_name_ = tmp;
        
        MQ mq_produce = MQ::create(mq_name_, O_WRONLY, 0600, MQ::Attr().set_maxmsg(5).set_msgsize(1));
        MQ mq_consume = MQ::open(mq_name_, O_RDONLY);

        char msg_sent = 'a';
        mq_produce.send(&msg_sent, 1, 0);
        char  msg_received = 0; // valgrind does not know about mq_receive()
        size_t nread = mq_consume.receive(&msg_received, 1);

        JFUNIT_ASSERT(nread == 1);
        JFUNIT_ASSERT(msg_received == 'a');
    }

private:
    std::string mq_name_;
};

// ------------------------------------------------------------
class UnrelatedProcessesUsingSameMQ : public jf::unittest::TestCase
{
public:
    UnrelatedProcessesUsingSameMQ() : jf::unittest::TestCase("UnrelatedProcessesUsingSameMQ") {}
    virtual void teardown()
    {
        MQ::unlink(mq_name_);
    }
    virtual void run()
    {
        char tmp[128];
        sprintf(tmp, "/MQSuite-Basic-%ld", random());
        mq_name_ = tmp;
        
        MQ::create(mq_name_, O_RDWR, 0600, MQ::Attr().set_maxmsg(5).set_msgsize(1));

        pid_t producer = fork();
        if (producer == 0) { // child
            MQ mq_produce = MQ::open(mq_name_, O_WRONLY);
            const char c = 'a'; 
            mq_produce.send(&c, 1, 0);
            _exit(0);
        }
        pid_t consumer = fork();
        if (consumer == 0) { // child
            MQ mq_consume = MQ::open(mq_name_, O_RDONLY);
            char c = 0; // valgrind does not know about mq_receive()
            size_t nread = mq_consume.receive(&c, 1);
            if (nread != 1)
                _exit(1);
            if (c != 'a')
                _exit(1);
            _exit(0);
        }

        JFUNIT_ASSERT(waitpid(producer, NULL, 0) == producer);
        JFUNIT_ASSERT(waitpid(consumer, NULL, 0) == consumer);
    }

private:
    std::string mq_name_;
};

}

namespace jf {
namespace linuxtools {

MQSuite::MQSuite()
: jf::unittest::TestSuite("MQ")
{
    add_test(std::auto_ptr<Test>(new BasicTest));
    add_test(std::auto_ptr<Test>(new UnrelatedProcessesUsingSameMQ));
}

}
}
