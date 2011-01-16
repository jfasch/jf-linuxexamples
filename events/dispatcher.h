// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2010-2011 Joerg Faschingbauer

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

#ifndef HAVE_JF_LINUXTOOLS_DISPATCHER_H
#define HAVE_JF_LINUXTOOLS_DISPATCHER_H

#include <map>

namespace jf {
namespace linuxtools {

class Dispatcher
{
public:
    class Handler
    {
    public:
        virtual void in_ready(int fd) = 0;
        virtual void out_ready(int fd) = 0;
    };
    
public:
    ~Dispatcher();

    void watch_in(int fd, Handler*);
    void watch_out(int fd, Handler*);

    void unwatch_in(int fd, const Handler*);
    void unwatch_out(int fd, const Handler*);

    void dispatch();

private:
    // should probably use hash_map instead, but gcc keeps giving me a
    // backward_warning. unordered_map, on the other hand, gives
    // c++0x_warning.h
    typedef std::map<int, Handler*> HandlerSet;
    HandlerSet in_handlers_;
    HandlerSet out_handlers_;
};

}
}

#endif
