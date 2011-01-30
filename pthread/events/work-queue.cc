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

#include "work-queue.h"

namespace jf {
namespace linuxtools {

WorkQueue::WorkQueue(size_t maxelem)
: queue_(maxelem)
{
    queue_.set_consumer(this);
}

void
WorkQueue::execute_work_async(
    std::auto_ptr<Work> work)
{
    queue_.push(_QueueData(work));
}

void
WorkQueue::execute_work_sync(
    Work& work)
{
    Future<bool> sync;
    queue_.push(_QueueData(&work, &sync));
    // synchronize with work execution.
    sync.get();
}

void
WorkQueue::activate_object(
    Dispatcher* dispatcher)
{
    queue_.activate_object(dispatcher);
}

void
WorkQueue::deactivate_object(
    const Dispatcher* dispatcher)
{
    queue_.deactivate_object(dispatcher);
}

size_t
WorkQueue::new_messages(
    const EventMessageQueue<_QueueData>* queue,
    size_t n_messages)
{
    _QueueData data;
    queue_.pop(data);
    data.work()->execute();
    if (data.sync())
        data.sync()->set(true);
    return 1;
}

}
}
