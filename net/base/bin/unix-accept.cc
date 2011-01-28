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

#include <jf/linuxtools/unix-port.h>

#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>

using namespace jf::linuxtools;
using namespace std;

static void usage(const char* argv0)
{
    cerr << "Usage: " << argv0 << " path-to-socket [mode-in-octal]" << endl;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        usage(argv[0]);
        exit(1);
    }
    const char* path = argv[1];

    bool have_mode = false;
    int mode;
    if (argc >= 3) {
        char* end;
        mode = strtol(argv[2], &end, 8);
        if (end == argv[2]) {
            cerr << "bad mode" << endl;
            exit(1);
        }
        have_mode = true;
    }

    UNIXPort port(path);
    if (have_mode && chmod(path, mode) < 0)
        throw ErrnoException(errno, "chmod()");

    port.listen();
    UNIXEndpoint connection = port.accept();

    FD std_out(STDOUT_FILENO); // :-)
    for (;;) {
        char tmp[64];
        ssize_t nread = connection.read(tmp, sizeof(tmp));
        if (nread == 0)
            exit(0);
        std_out.write(tmp, nread);
    }
    return 0;
}
