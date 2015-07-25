// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2015 Joerg Faschingbauer

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

#ifndef HAVE_JF_LINUXTOOLS_THREADPOOL_H
#define HAVE_JF_LINUXTOOLS_THREADPOOL_H

#include "joinable-thread.h"
#include "message-queue.h"

#include <vector>


namespace jf {
namespace linuxtools {

class ThreadPool
{
public:
    class Command
    {
    public:
        virtual ~Command() {}
        virtual void execute() = 0;
    };

public:
    ThreadPool(size_t numthreads);
    ~ThreadPool();

    // execute the command. returns once the command has been fully
    // executed.
    void execute(Command *);

public:
    // this is like a future, but with no data. simply carries a
    // "done" flag which is waited upon.
    class Synchronizer
    {
    public:
        Synchronizer();
        void wait();
        void done();

    private:
        bool is_done_;
        Mutex lock_;
        Condition done_;
    };
    
private:
    // the queue which we send commands and their syncers over.
    MessageQueue<std::pair<Command*, class Synchronizer*> > command_queue_;

    // threads and their worker objects
    typedef std::vector<std::pair<JoinableThreadStarter*,JoinableThreadStarter::Worker*> > ThreadsAndWorkers;
    ThreadsAndWorkers threads_and_workers_;
};

}
}

#endif
