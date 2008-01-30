// -*- mode: C++; c-basic-offset: 4 -*-

#include "basic_thread_test.h"

#include <jf/unix_tools/joinable_thread.h>

namespace {

class TestWorker : public jf::unix_tools::JoinableThreadStarter::Worker {
public:
    TestWorker() : was_running_(false) {}
    bool was_running() const { return was_running_; }

    virtual void operator()() {
        was_running_ = true;
    }

private:
    bool was_running_;
};

}

namespace jf {
namespace unix_tools {
namespace tests {

void BasicThreadTest::run()
{
    TestWorker* worker = new TestWorker;
    JoinableThreadStarter t(worker);
    t.start();
    t.join();
    JFUNIT_ASSERT(worker->was_running());
}

}
}
}
