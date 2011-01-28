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

#include "worker-thread.h"

namespace jf {
namespace linuxtools {

WorkerThread::WorkerThread(size_t maxelem)
: queue_(maxelem),
  worker_(&queue_),
  thread_(&worker_) {}

void
WorkerThread::start()
{
    thread_.start();
}

void
WorkerThread::stop()
{
    class Quit : public Work
    {
    public:
        Quit(_Worker* worker) : worker_(worker) {}
        virtual void execute() { worker_->quit(); }
    private:
        _Worker* worker_;
    };

    Quit quit(&worker_);
    queue_.execute_work_sync(quit);
    thread_.join();
}

void
WorkerThread::execute_work_async(
    std::auto_ptr<Work> work)
{
    queue_.execute_work_async(work);
}

void
WorkerThread::execute_work_sync(
    Work& work)
{
    queue_.execute_work_sync(work);
}

// --------------------------------------------
void WorkerThread::_Worker::run()
{
    queue_->activate_object(&dispatcher_);
    try {
        while (!quit_)
            dispatcher_.dispatch();
    }
    catch (std::exception&) {
        // TODO: error handling. best to remember the exception for
        // the other side to throw.
        assert(false);
    }
    queue_->deactivate_object(&dispatcher_);
}

}
}
