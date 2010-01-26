// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2010 Joerg Faschingbauer

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

#include "dispatcher.h"

#include <jflinux/error.h>

#include <cassert>
#include <sys/select.h>
#include <errno.h>

namespace jflinux {

Dispatcher::~Dispatcher()
{
    assert(in_handlers_.size()==0);
    assert(out_handlers_.size()==0);
}

void Dispatcher::watch_in(int fd, Handler* h)
{
    std::pair<HandlerSet::iterator, bool> found =
        in_handlers_.insert(std::make_pair(fd, h));
    assert(found.second==true);
}

void Dispatcher::watch_out(int fd, Handler* h)
{
    std::pair<HandlerSet::iterator, bool> found =
        out_handlers_.insert(std::make_pair(fd, h));
    assert(found.second==true);
}

void Dispatcher::unwatch_in(int fd, const Handler* h)
{
    HandlerSet::iterator found = in_handlers_.find(fd);
    assert(found!=in_handlers_.end());
    assert(found->second==h);
    in_handlers_.erase(found);
}

void Dispatcher::unwatch_out(int fd, const Handler* h)
{
    HandlerSet::iterator found = out_handlers_.find(fd);
    assert(found!=out_handlers_.end());
    assert(found->second==h);
    out_handlers_.erase(found);
}

void Dispatcher::dispatch()
{
    int max_fd = 0;
    
    fd_set in_fds;
    FD_ZERO(&in_fds);
    for (HandlerSet::iterator i = in_handlers_.begin();
         i != in_handlers_.end();
         ++i) {
        FD_SET(i->first, &in_fds);
        max_fd = std::max(max_fd, i->first);
    }

    fd_set out_fds;
    FD_ZERO(&out_fds);
    for (HandlerSet::iterator i = out_handlers_.begin();
         i != out_handlers_.end();
         ++i) {
        FD_SET(i->first, &out_fds);
        max_fd = std::max(max_fd, i->first);
    }

    // save away handler sets for later usage as the live ones may be
    // changed during the callback phase. this should probably be
    // optimized a bit as this is done quite often.
    HandlerSet saved_in_handlers(in_handlers_);
    HandlerSet saved_out_handlers(out_handlers_);

    int retval = ::select(max_fd+1, &in_fds, &out_fds, NULL, NULL);
    if (retval < 0) {
        if (errno == EINTR)
            return;
        throw ErrnoException(errno);
    }
    assert(retval>0); // we don't pass a timeout

    for (int fd=0; fd<=max_fd; fd++)
        if (FD_ISSET(fd, &in_fds)) {
            HandlerSet::const_iterator found = saved_in_handlers.find(fd);
            assert(found!=saved_in_handlers.end());
            found->second->in_ready(fd);
        }
    for (int fd=0; fd<=max_fd; fd++)
        if (FD_ISSET(fd, &out_fds)) {
            HandlerSet::const_iterator found = saved_out_handlers.find(fd);
            assert(found!=saved_out_handlers.end());
            found->second->out_ready(fd);
        }
}

}
