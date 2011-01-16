// -*- C++ -*-

#include "ipaddress.h"

#include <cstring>

namespace jf {
namespace linuxtools {

static inline unsigned long make_addr(const char* addr_string)
{
   unsigned long addr = ::inet_addr(addr_string) ;
   if (addr == INADDR_NONE)
       throw IPAddress::BadAddress(addr_string);
   return addr;
}

IPAddress IPAddress::any(INADDR_ANY);

IPAddress::BadAddress::BadAddress(const std::string& bad_address_string)
{
    message_ = "Bad address string: ";
    message_ += bad_address_string;
}

IPAddress::IPAddress()
{
    std::memset(this, 0, sizeof(*this));
}

IPAddress::IPAddress(in_addr a)
{
    std::memcpy(this, &a, sizeof(a));
}

IPAddress::IPAddress(uint32_t a)
{
    this->s_addr = a;
}

IPAddress::IPAddress(const char* hostaddr)
{
    this->s_addr = make_addr(hostaddr);
}

IPAddress::IPAddress(const std::string& hostaddr)
{
    this->s_addr = make_addr(hostaddr.c_str());
}

std::string IPAddress::host_string() const
{
    return std::string(::inet_ntoa(*this));
}

}
}
