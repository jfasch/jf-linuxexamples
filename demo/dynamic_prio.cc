// -*- mode: C++; c-basic-offset: 4 -*-

#include <jf/linux/joinable_thread.h>

#include <unistd.h>

using namespace jf::linux;

namespace {

class SleeperWorker : public JoinableThreadStarter::Worker
{
public:
    virtual void run()
    {
        ::pause();
    }
};

class SpinnerWorker : public JoinableThreadStarter::Worker
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
    JoinableThreadStarter sleeper(JoinableThreadStarter::Args().worker(new SleeperWorker));
    JoinableThreadStarter spinner(JoinableThreadStarter::Args().worker(new SpinnerWorker));
    sleeper.start();
    spinner.start();
    return 0;
}
