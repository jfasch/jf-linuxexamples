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

#include <jf/linuxtools/unix-connect.h>

#include <iostream>

using namespace jf::linuxtools;
using namespace std;

static void usage(const char* argv0)
{
    cerr <<
        "Usage: " << argv0 << " path-to-socket\n"
        "       Connect to socket\n"
        "       Until eof:\n"
        "         Read from standard input\n"
        "         Write to socket";
    cerr << endl;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        usage(argv[0]);
        exit(1);
    }
    const char* path = argv[1];

    UNIXEndpoint connection = unix_connect(path);
    FD std_in(STDIN_FILENO); // :-)

    for (;;) {
        char tmp[64];
        size_t nread = std_in.read(tmp, sizeof(tmp));
        if (nread == 0)
            exit(0);
        connection.writeall(tmp, nread);
    }
    return 0;
}
