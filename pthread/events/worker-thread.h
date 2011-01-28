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

#ifndef HAVE_JF_LINUXTOOLS_WORKER_THREAD_H
#define HAVE_JF_LINUXTOOLS_WORKER_THREAD_H

#include <jf/linuxtools/joinable-thread.h>

#include "work-queue.h"

namespace jf {
namespace linuxtools {

class WorkerThread
{
public:
    WorkerThread(size_t maxelem);

    void start();
    void stop();

    /** \brief Execute work, without waiting.

        The work is enqueued, and execution resumes immediately
        (provided that there is enough room in the queue).  Ownership
        is taken of the Work object, and the object is deleted once it
        has been executed.
    */
    void execute_work_async(std::auto_ptr<Work>);

    /** \brief Execute work and wait for it.

        The work is enqueued, and execution is suspended until the
        work has executed.
    */
    void execute_work_sync(Work&);

private:
    // executed inside the thread, doing the real work.
    class _Worker : public JoinableThreadStarter::Worker
    {
    public:
        _Worker(WorkQueue* q) : quit_(false), queue_(q) {}
        void quit() { quit_ = true; }
        virtual void run();
    private:
        bool quit_;
        WorkQueue* queue_;
        Dispatcher dispatcher_;
    };

    WorkQueue queue_;
    _Worker worker_;
    JoinableThreadStarter thread_;
};

}
}

#endif
