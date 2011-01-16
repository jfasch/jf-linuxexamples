// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2011 Joerg Faschingbauer

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

#include "tty.h"

namespace jf {
namespace linuxtools {

static inline void get_termios(int fd, termios& t)
{
    if (::tcgetattr(fd, &t) < 0)
        throw ErrnoException(errno, "tcgetattr()");
}

static inline void set_termios(int fd, termios& t)
{
    if (::tcsetattr(fd, TCSADRAIN, &t) < 0)
        throw ErrnoException(errno, "tcsetattr()");
}

static inline bool apply_raw(termios& t, TTY::Raw r)
{
    switch (r) {
        case TTY::R_YES:
            cfmakeraw(&t);
            return true;
        case TTY::R_NOCHANGE:
            return false;
    }
    assert(false);
    return false;
}

static inline bool apply_speed(termios& t, TTY::Baudrate s)
{
    speed_t speed;
    switch (s) {
        case TTY::B_9600: speed = B9600; break;
        case TTY::B_19200: speed = B19200; break;
        case TTY::B_38400: speed = B38400; break;
        case TTY::B_57600: speed = B57600; break;
        case TTY::B_115200: speed = B115200; break;
        case TTY::B_NOCHANGE: return false;
    }
    if (::cfsetispeed(&t, speed) < 0)
        throw ErrnoException(errno, "cfsetispeed()");
    if (::cfsetospeed(&t, speed) < 0)
        throw ErrnoException(errno, "cfsetospeed()");
    return true;
}

static inline bool apply_charsize(termios& t, TTY::CharacterSize s)
{
#   define TTY_SETCS(t, s) { t.c_cflag &= ~CSIZE; t.c_cflag |= s; }
    switch (s) {
        case TTY::CS_5:
            TTY_SETCS(t, CS5);
            return true;
        case TTY::CS_6:
            TTY_SETCS(t, CS6);
            return true;
        case TTY::CS_7:
            TTY_SETCS(t, CS7);
            return true;
        case TTY::CS_8:
            TTY_SETCS(t, CS8);
            return true;
        case TTY::CS_NOCHANGE:
            return false;
    }
    assert(false);
    return false;
}

static inline bool apply_stopbits(termios& t, TTY::StopBits s)
{
    switch (s) {
        case TTY::SB_ONE:
            t.c_cflag &= ~CSTOPB;
            return true;
        case TTY::SB_TWO:
            t.c_cflag |= CSTOPB;
            return true;
        case TTY::SB_NOCHANGE:
            return false;
    }
    assert(false);
    return false;
}

static inline bool apply_parity(termios& t, TTY::Parity p)
{
    switch (p) {
        case TTY::PAR_NONE:
            t.c_cflag &= ~PARENB;
            return true;
        case TTY::PAR_ODD:
            t.c_cflag |= PARENB;
            t.c_cflag |= PARODD;
            return true;
        case TTY::PAR_EVEN:
            t.c_cflag |= PARENB;
            t.c_cflag &= ~PARODD;
            return true;
        case TTY::PAR_NOCHANGE:
            return false;
    }
    assert(false);
    return false;
}

void TTY::open(const char* pathname, int flags, const Config& config)
{
    File::open(pathname, flags);
    assert(good()); // paranoia: should have thrown on error

    try {
        get_termios(fd(), termios_);
        
        bool changed;
        changed = apply_raw(termios_, config.raw());
        changed = apply_speed(termios_, config.baudrate());
        changed = apply_charsize(termios_, config.charsize());
        changed = apply_stopbits(termios_, config.stopbits());
        changed = apply_parity(termios_, config.parity());

        if (changed)
            set_termios(fd(), termios_);
    }
    catch (const std::exception&) {
        close();
        throw;
    }
}
    
}
}
