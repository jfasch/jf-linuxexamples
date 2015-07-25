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

#include "threadpool.h"
#include "mutex.h"

#include <cassert>


namespace {

using namespace jf::linuxtools;

// special command type for CommandExecutor termination.
class TerminateCommand : public ThreadPool::Command
{
public:
    virtual void execute() {}
};


// the worker, sitting inside each thread that we create. pulls
// commands (and their associated syncers) from the queue, executes
// the command, and signals the syncer (waking the client)
class CommandExecutor : public JoinableThreadStarter::Worker
{
public:
    CommandExecutor(MessageQueue<std::pair<ThreadPool::Command*, class ThreadPool::Synchronizer*> > *queue)
    : queue_(queue) {}

    virtual void run()
    {
        while (true) {
            std::pair<ThreadPool::Command*, class ThreadPool::Synchronizer*> command_and_sync;
            queue_->pop(command_and_sync);
            command_and_sync.first->execute();
            command_and_sync.second->done();

            // see if we received the special termination command
            if (dynamic_cast<TerminateCommand*>(command_and_sync.first))
                break;
        }
    }

private:
    MessageQueue<std::pair<ThreadPool::Command*, class ThreadPool::Synchronizer*> > *queue_;
};

}

namespace jf {
namespace linuxtools {

// ----------------------------------------------------------
ThreadPool::Synchronizer::Synchronizer() 
: is_done_(false) {}

void ThreadPool::Synchronizer::wait()
{
    Mutex::Guard guard(lock_);
    while (!is_done_)
        done_.wait(lock_);
}
void ThreadPool::Synchronizer::done()
{
    Mutex::Guard guard(lock_);
    is_done_ = true;
    done_.signal();
}

// ----------------------------------------------------------
ThreadPool::ThreadPool(size_t numthreads)
: command_queue_(10)
{
    while (numthreads--) {
        JoinableThreadStarter::Worker *worker = new CommandExecutor(&command_queue_);
        JoinableThreadStarter *thread = new JoinableThreadStarter(worker);
        threads_and_workers_.push_back(std::make_pair(thread, worker));
        bool rv = thread->start();
        assert(rv);
    }
}

ThreadPool::~ThreadPool()
{
    // terminate all threads, join with them, and cleanup
    TerminateCommand terminate;
    for (ThreadsAndWorkers::iterator i=threads_and_workers_.begin(); i!=threads_and_workers_.end(); ++i) {
        execute(&terminate);
        i->first->join();
        delete i->first;
        delete i->second;
    }
}

void ThreadPool::execute(ThreadPool::Command *command)
{
    Synchronizer syncer;
    command_queue_.push(std::make_pair(command, &syncer));

    // wait until we are sure that the work is done
    syncer.wait();
}

}
}
