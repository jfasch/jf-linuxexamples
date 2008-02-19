// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008 Joerg Faschingbauer

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

#ifndef HAVE_JFLINUX_DEBUG_STACK_HISTORY_IMPL_H
#define HAVE_JFLINUX_DEBUG_STACK_HISTORY_IMPL_H

#include <jflinux/timespec.h>
#include <jflinux/pthread/thread_specific.h>

#include <vector>

namespace jflinux {
namespace debug {

template <typename FRAMEDESCRIPTION> class StackElement
{
public:
    StackElement() {}
    StackElement(FRAMEDESCRIPTION description)
    : description_(description),
      enter_(jflinux::TimeSpec::now()) {}
    ~StackElement()
    {
        for (unsigned i=0; i<descendants_.size(); i++)
            delete descendants_[i];
    }

    const FRAMEDESCRIPTION& description() const { return description_; }
    const jflinux::TimeSpec& enter() const { return enter_; }
    const jflinux::TimeSpec& leave() const { return leave_; }
    const std::vector<StackElement*> descendants() const { return descendants_; }

    void add_descendant(StackElement* e)
    {
        descendants_.push_back(e);
    }

    void terminate()
    {
        leave_ = jflinux::TimeSpec::now();
    }

private:
    FRAMEDESCRIPTION description_;
    jflinux::TimeSpec enter_;
    jflinux::TimeSpec leave_;
    std::vector<StackElement*> descendants_;
};

template <typename FRAMEDESCRIPTION> class StackHistory
{
public:
    static StackHistory* instance()
    {
        return history_instance.get();
    }

    static void set_instance(StackHistory* h)
    {
        history_instance.set(h);
    }

public:
    StackHistory() : current_(&root_) {}

    const StackElement<FRAMEDESCRIPTION>& root() const { return root_; }

    StackElement<FRAMEDESCRIPTION>* /*predecessor*/ push(const FRAMEDESCRIPTION& desc)
    {
        StackElement<FRAMEDESCRIPTION>* new_element = new StackElement<FRAMEDESCRIPTION>(desc);
        current_->add_descendant(new_element);
        StackElement<FRAMEDESCRIPTION>* predecessor = current_;
        current_ = new_element;
        return predecessor;
    }

    void pop(StackElement<FRAMEDESCRIPTION>* predecessor)
    {
        current_->terminate();
        current_ = predecessor;
    }

private:
    StackElement<FRAMEDESCRIPTION> root_;
    StackElement<FRAMEDESCRIPTION>* current_;

    static jflinux::pthread::ThreadSpecific<StackHistory> history_instance;
};

template <typename FRAMEDESCRIPTION>
jflinux::pthread::ThreadSpecific<StackHistory<FRAMEDESCRIPTION> > StackHistory<FRAMEDESCRIPTION>::history_instance;

template <typename FRAMEDESCRIPTION> class StackFrame
{
public:
    StackFrame(FRAMEDESCRIPTION description, StackHistory<FRAMEDESCRIPTION>* history)
    : history_(history),
      predecessor_(NULL)
    {
        if (history_)
            predecessor_ = history_->push(description);
    }
    ~StackFrame()
    {
        if (history_)
            history_->pop(predecessor_);
    }

private:
    StackHistory<FRAMEDESCRIPTION>* history_;
    StackElement<FRAMEDESCRIPTION>* predecessor_;
};

}
}

#endif
