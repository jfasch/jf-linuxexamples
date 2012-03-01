// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2012 Joerg Faschingbauer

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

#include "suite.h"

#include <jf/unittest/tree.h>
#include <jf/unittest/find.h>

#include <iostream>
#include <cassert>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>

using namespace jf::unittest;

int main(int argc, char** argv)
{
    bool print_path = false;
    char opt;
    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt) {
            case 'p':
                print_path = true;
                break;
            default:
                assert(false);
                exit(1);
                break;
        }
    }

    const char* path = (optind < argc)? argv[optind]: NULL;

    TestSuite root;
    TestSuite* suite = new jf::linuxtools::Suite;
    root.add_test(std::auto_ptr<Test>(suite));

    Test* run_test = suite;
    if (path != NULL) {
        run_test = find(&root, path);
        if (run_test == NULL) {
            std::cerr << "Path not found: " << path << std::endl;
            return 1;
        }
    }
    jf::unittest::TreeWalk tree_walk(std::cout);
    tree_walk.print_path(print_path).use_fork(true);
    if (tree_walk.do_it(*run_test))
        return 0;
    else
        return 1;
}
