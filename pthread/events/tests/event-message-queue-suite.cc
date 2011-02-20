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

#include "event-message-queue-suite.h"

#include <jf/linuxtools/event-message-queue.h>
#include <jf/linuxtools/joinable-thread.h>

#include <jf/unittest/test_case.h>

#include <cassert>

namespace {

using namespace jf::linuxtools;

// -------------------------------------------------------------------
class BasicTest : public jf::unittest::TestCase
{
public:
    BasicTest() : jf::unittest::TestCase("Basic") {}

    virtual void run()
    {
        class MyConsumer : public EventMessageQueue<int>::Consumer
        {
        public:
            MyConsumer() : n_messages_(0) {}
            size_t n_messages() const { return n_messages_; }
            virtual size_t new_messages(const EventMessageQueue<int>*, size_t n)
            {
                n_messages_ += n;
                return n;
            }
        private:
            size_t n_messages_;
        };
        
        EventMessageQueue<int> mq(5);

        MyConsumer consumer;
        mq.set_consumer(&consumer);
        
        Dispatcher dispatcher;
        mq.activate_object(&dispatcher);

        mq.push(1001);
        while (consumer.n_messages() != 1)
            dispatcher.dispatch();

        int i;
        mq.pop(i);
        JFUNIT_ASSERT(i == 1001);

        mq.deactivate_object(&dispatcher);
    }
};

// -------------------------------------------------------------------
class MultithreadedTestAMillionMessages : public jf::unittest::TestCase
{
public:
    MultithreadedTestAMillionMessages() : jf::unittest::TestCase("MultithreadedTestAMillionMessages") {}

    virtual void run()
    {
        class MyWorker : public JoinableThreadStarter::Worker
        {
        public:
            MyWorker(size_t expect_messages_per_queue, EventMessageQueue<int>* queue1, EventMessageQueue<int>* queue2)
            : queue1_(queue1),
              queue2_(queue2),
              expect_messages_per_queue_(expect_messages_per_queue) {}
            
            virtual void run()
            {
                class MyConsumer : public EventMessageQueue<int>::Consumer
                {
                public:
                    MyConsumer(EventMessageQueue<int>* queue)
                    : queue_(queue),
                      total_messages_(0) {}

                    size_t total_messages() const { return total_messages_; }
                    
                    virtual size_t new_messages(const EventMessageQueue<int>* queue, size_t n_messages)
                    {
                        assert(queue == queue_);

                        size_t rv = n_messages;
                        total_messages_ += n_messages;
                        while (n_messages--) {
                            int i;
                            queue_->pop(i);
                        }

                        return rv;
                    }
                    
                private:
                    EventMessageQueue<int>* queue_;
                    size_t total_messages_;
                };


                MyConsumer consumer1(queue1_);
                MyConsumer consumer2(queue2_);
                queue1_->set_consumer(&consumer1);
                queue2_->set_consumer(&consumer2);

                Dispatcher dispatcher;
                queue1_->activate_object(&dispatcher);
                queue2_->activate_object(&dispatcher);

                while (! (consumer1.total_messages() == expect_messages_per_queue_ && 
                          consumer2.total_messages() == expect_messages_per_queue_))
                    dispatcher.dispatch();

                queue1_->deactivate_object(&dispatcher);
                queue2_->deactivate_object(&dispatcher);
            }

        private:
            size_t expect_messages_per_queue_;
            EventMessageQueue<int>* queue1_;
            EventMessageQueue<int>* queue2_;
        };


        EventMessageQueue<int> queue1(5);
        EventMessageQueue<int> queue2(5);

        MyWorker worker(100000, &queue1, &queue2);

        JoinableThreadStarter starter(&worker);
        bool rv = starter.start();
        assert(rv); // start() should void and throw instead.

        for (int i=0; i<100000; i++) {
            queue1.push(i);
            queue2.push(i);
        }

        starter.join();
    }
};

// -------------------------------------------------------------------
class Reannounce : public jf::unittest::TestCase
{
public:
    Reannounce() : jf::unittest::TestCase("Reannounce") {}

    virtual void run()
    {
        // consumes only one message on each call, and lets the rest
        // (whatever that may be) for reannouncement.
        class MyConsumer : public EventMessageQueue<int>::Consumer
        {
        public:
            MyConsumer() : n_messages_(0) {}
            size_t n_messages() const { return n_messages_; }
            virtual size_t new_messages(const EventMessageQueue<int>*, size_t n)
            {
                n_messages_++;
                return 1;
            }
        private:
            size_t n_messages_;
        };
        
        EventMessageQueue<int> mq(1000);

        MyConsumer consumer;
        mq.set_consumer(&consumer);
        
        Dispatcher dispatcher;
        mq.activate_object(&dispatcher);

        for (int i=0; i<1000; i++)
            mq.push(i);
        while (consumer.n_messages() != 1000)
            dispatcher.dispatch();

        mq.deactivate_object(&dispatcher);
    }
};

}

namespace jf {
namespace linuxtools {

EventMessageQueueSuite::EventMessageQueueSuite()
: jf::unittest::TestSuite("EventMessageQueue")
{
    add_test(std::auto_ptr<Test>(new BasicTest));
    add_test(std::auto_ptr<Test>(new MultithreadedTestAMillionMessages));
    add_test(std::auto_ptr<Test>(new Reannounce));
}

}
}
