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

#include "mq-suite.h"

#include <jf/linuxtools/mq.h>

#include <jf/unittest/test_case.h>

#include <stdlib.h>
#include <time.h>
#include <mqueue.h>

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

class BasicTest : public jf::unittest::TestCase
{
public:
    BasicTest() : jf::unittest::TestCase("Basic") {}
    virtual void teardown()
    {
        mq_unlink(mqname_.c_str());
    }
    virtual void run()
    {
        char tmp[128];
        sprintf(tmp, "/MQSuite-Basic-%ld", random());
        mqname_ = tmp;
        
        MQ mq_produce = MQ::create(mqname_.c_str(), O_WRONLY, 0600, MQ::Attr().set_maxmsg(5).set_msgsize(1));
        MQ mq_consume = MQ::open(mqname_.c_str(), O_RDONLY);

        char msg_sent = 'a';
        mq_produce.send(&msg_sent, 1, 0);
        char  msg_received;
        size_t nread = mq_consume.receive(&msg_received, 1);

        JFUNIT_ASSERT(nread == 1);
        JFUNIT_ASSERT(msg_received == 'a');
    }

private:
    std::string mqname_;
};

}

namespace jf {
namespace linuxtools {

MQSuite::MQSuite()
: jf::unittest::TestSuite("MQ")
{
    add_test(new BasicTest);
}

}
}
