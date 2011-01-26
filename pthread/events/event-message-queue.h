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
#ifndef HAVE_JF_LINUXTOOLS_EVENT_MESSAGE_QUEUE_H
#define HAVE_JF_LINUXTOOLS_EVENT_MESSAGE_QUEUE_H

#include <jf/linuxtools/dispatchee.h>
#include <jf/linuxtools/message-queue.h>
#include <jf/linuxtools/event.h>

namespace jf {
namespace linuxtools {

/** A message queue is event driven on the consumer side.

    This message queue implementation resembles the interface (and
    implementation) of MessageQueue. On the consumer side, it can be
    hooked in a Dispatcher instance, for the purpose of getting
    notifications when objects arrive at the producer side.

    The consumer (EventMessageQueue<T>::Consumer) receives callbacks
    that carry the number of new messages. Messages are announced only
    once, so care must be taken to correctly consume exactly the
    amount of messages that have been announced. Otherwise, deadlocks
    will happen.
 */
template<typename T>
class EventMessageQueue : public Dispatchee, private Event::Handler
{
public:
    class Consumer
    {
    public:
        virtual ~Consumer() {}
        virtual void new_messages(const EventMessageQueue<T>* queue, size_t n_messages) = 0;
    };
    
public:
    EventMessageQueue(size_t maxelem)
    : consumer_(NULL),
      queue_(maxelem),
      event_(this) {}

    void set_consumer(Consumer* consumer)
    {
        assert(consumer_==NULL);
        consumer_ = consumer;
    }

    void push(const T& elem)
    {
        queue_.push(elem);
        event_.add(1);
    }

    void pop(T& elem)
    {
        queue_.pop(elem);
    }

public:
    /** Dispatchee implementation. */
    //@{
    virtual void activate_object(Dispatcher* dispatcher)
    {
        assert(consumer_!=NULL);
        event_.activate_object(dispatcher);
    }
    virtual void deactivate_object(const Dispatcher* dispatcher)
    {
        event_.deactivate_object(dispatcher);
    }
    //@}

private:
    /** Event::Handler implementation. */
    //@{
    virtual void new_events(const Event*, uint64_t n)
    {
        consumer_->new_messages(this, n);
    }
    //@}

private:
    Consumer* consumer_;
    MessageQueue<T> queue_;
    Event event_;
};

}
}

#endif
