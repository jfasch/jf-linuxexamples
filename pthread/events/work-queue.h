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

#ifndef HAVE_JF_LINUXTOOLS_WORK_QUEUE_H
#define HAVE_JF_LINUXTOOLS_WORK_QUEUE_H

#include "event-message-queue.h"
#include "work.h"

#include <jf/linuxtools/future.h>

#include <memory>

namespace jf {
namespace linuxtools {

/** \brief Internal WorkQueue helper.

    Too bad it has to take so much space, right here.
 */
class _QueueData
{
public:
    _QueueData() {}
    _QueueData(Work* work, Future<bool>* sync)
    : work_ptr_(work),
      sync_(sync) {}
    _QueueData(std::auto_ptr<Work> ptr)
    : work_autoptr_(ptr),
      work_ptr_(NULL),
      sync_(NULL) {}
    _QueueData(const _QueueData& d)
    : work_ptr_(d.work_ptr_),
      work_autoptr_(d.work_autoptr_),
      sync_(d.sync_) {}
    _QueueData& operator=(const _QueueData& d)
    {
        work_ptr_ = d.work_ptr_;
        work_autoptr_ = d.work_autoptr_;
        sync_ = d.sync_;
        return *this;
    }

    Future<bool>* sync() const { return sync_; }
    Work* work() const { return work_ptr_? work_ptr_: work_autoptr_.get(); }

private:
    Work* work_ptr_;
    mutable std::auto_ptr<Work> work_autoptr_;
    Future<bool>* sync_;
};

class WorkQueue : public Dispatchee,
                  private EventMessageQueue<_QueueData>::Consumer
{
public:
    WorkQueue(size_t maxelem);

    /** \brief Execute work at opposite queue end, without waiting.

        The work is enqueued, and execution resumes immediately
        (provided that there is enough room in the queue). The queue
        takes ownership of the Work object, and the object is deleted
        once it has been executed.
    */
    void execute_work_async(std::auto_ptr<Work>);

    /** \brief Execute work at opposite queue end, and wait for it.

        The work is enqueued, and execution is suspended until the
        work has executed. Obviously the queue has two live in two
        different threads for this to work, or otherwise a deadlock
        will occur.
    */
    void execute_work_sync(Work&);

public:
    /** Dispatchee implementation */
    //@{
    virtual void activate_object(Dispatcher*);
    virtual void deactivate_object(const Dispatcher*);
    //@}

private:
    /** EventMessageQueue<_QueueData>::Consumer implementation */
    //@{
    virtual size_t new_messages(const EventMessageQueue<_QueueData>*, size_t);
    //@}

private:
    EventMessageQueue<_QueueData> queue_;
};

}
}

#endif
