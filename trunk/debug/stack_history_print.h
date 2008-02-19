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

#ifndef HAVE_JFLINUX_DEBUG_STACK_HISTORY_PRINT_H
#define HAVE_JFLINUX_DEBUG_STACK_HISTORY_PRINT_H

#include "indent.h"
#include "stack_history_impl.h"

namespace jflinux {
namespace debug {

template <typename FRAMEDESCRIPTION> void print(std::ostream& o, const StackElement<FRAMEDESCRIPTION>& e, int indentation)
{
    o << indent(indentation) << "* " << e.description() << '\n';
    o << indent(indentation+2) << "enter: " << e.enter().sec() << ':' << e.enter().nsec() << '\n';
    o << indent(indentation+2) << "leave: " << e.leave().sec() << ':' << e.leave().nsec() << '\n';
    if (e.leave() < e.enter())
        o << indent(indentation+2) << "total: (skew detected)" << '\n';
    else {
        jflinux::TimeSpec total(e.leave() - e.enter());
        o << indent(indentation+2) << "total: " << total.sec() << ':' << total.nsec() << '\n';
    }
    for (unsigned int i=0; i<e.descendants().size(); i++)
        print(o, *e.descendants()[i], indentation+2);
}

template <typename FRAMEDESCRIPTION> inline void print(std::ostream& o, const StackHistory<FRAMEDESCRIPTION>& h, int indentation=0)
{
    print(o, h.root(), indentation);
}

}
}

#endif
