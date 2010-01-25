// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2010 Joerg Faschingbauer

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

#ifndef HAVE_JFLINUX_TTY_H
#define HAVE_JFLINUX_TTY_H

#include <jflinux/file.h>

#include <termios.h>

namespace jflinux {

class TTY : public jflinux::File
{
public:
    enum Raw {
      R_YES,
      R_NOCHANGE
    };
    enum Baudrate {
      B_9600,
      B_19200,
      B_38400,
      B_57600,
      B_115200,
      B_NOCHANGE
    };
    enum CharacterSize {
      CS_5,
      CS_6,
      CS_7,
      CS_8,
      CS_NOCHANGE
    };
    enum StopBits {
      SB_ONE,
      SB_TWO,
      SB_NOCHANGE
    };
    enum Parity {
      PAR_NONE,
      PAR_ODD,
      PAR_EVEN,
      PAR_NOCHANGE
    };

public:
    class Config
    {
    public:
        Config()
        : raw_(R_NOCHANGE),
          baudrate_(B_NOCHANGE),
          charsize_(CS_NOCHANGE),
          stopbits_(SB_NOCHANGE),
          parity_(PAR_NOCHANGE) {}

        Config& raw(Raw r) { raw_=r; return *this; }
        Config& baudrate(Baudrate b) { baudrate_=b; return *this; }
        Config& charsize(CharacterSize c) { charsize_=c; return *this; }
        Config& stopbits(StopBits s) { stopbits_=s; return *this; }
        Config& parity(Parity p) { parity_=p; return *this; }

        Raw raw() const { return raw_; }
        Baudrate baudrate() const { return baudrate_; }
        CharacterSize charsize() const { return charsize_; }
        StopBits stopbits() const { return stopbits_; }
        Parity parity() const { return parity_; }

    private:
        Raw raw_;
        Baudrate baudrate_;
        CharacterSize charsize_;
        StopBits stopbits_;
        Parity parity_;
    };
public:
    void open(const std::string& pathname, int flags, const Config& c) { open(pathname.c_str(), flags, c); }
    void open(const char* pathname, int flags, const Config&);

private:
    termios termios_;
};

}

#endif
