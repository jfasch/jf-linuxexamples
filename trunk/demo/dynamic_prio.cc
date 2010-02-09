// -*- mode: C++; c-basic-offset: 4 -*-

#include <jf/linux/joinable_thread.h>

#include <unistd.h>

using namespace jf::linux;

namespace {

class Sleeper : public JoinableThreadStarter::Worker
{
public:
    virtual void run()
    {
        ::pause();
    }
};

class Spinner : public JoinableThreadStarter::Worker
{
public:
    virtual void run()
    {
        for (;;);
    }
};

}

int main()
{
    JoinableThreadStarter sleeper(JoinableThreadStarter::Args().worker(new Sleeper));
    JoinableThreadStarter spinner(JoinableThreadStarter::Args().worker(new Spinner));
    sleeper.start();
    spinner.start();
    return 0;
}
