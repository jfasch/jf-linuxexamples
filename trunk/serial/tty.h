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

#ifndef HAVE_JF_LINUX_SERIAL_TTY_H
#define HAVE_JF_LINUX_SERIAL_TTY_H

#include <jflinux/io/io.h>

namespace jflinux {
namespace serial {

class TTY : public jflinux::io::File
{
public:
    class Config
    {
    public:
        enum Baudrate {
          B_9600,
          B_19200,
          B_38400,
          B_57600,
          B_115200
        };
        enum CharacterSize {
          CS_5,
          CS_6,
          CS_7,
          CS_8          
        };
        enum StopBits {
          SB_ONE,
          SB_TWO
        };
        enum Parity {
          PAR_NONE,
          PAR_ODD,
          PAR_EVEN
        };
    public:
        Config() : baudrate_(B_115200), charsize_(CS_8) {}
        Config& baudrate(Baudrate b) { baudrate_=b; return *this; }
        Config& charsize(CharacterSize c) { charsize_=c; return *this; }
        Config& stopbits(StopBits s) { stopbits_=s; return *this; }
        Config& parity(Parity p) { parity_=p; return *this; }

    private:
        Baudrate baudrate_;
        CharacterSize charsize_;
        StopBits stopbits_;
        Parity parity_;
    };
public:
    TTY(const Config& config) : config_(config) {}

    
    
};
    
}
}

#endif
