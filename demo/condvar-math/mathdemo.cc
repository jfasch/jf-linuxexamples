#include "value.h"
#include "channel.h"
#include "channel_group.h"
#include "producer.h"
#include "timebase.h"

#include <jf/linuxtools/timespec.h>
#include <jf/linuxtools/joinable-thread.h>

#include <iostream>
#include <time.h>


using jf::linuxtools::TimeSpec;
using jf::linuxtools::JoinableThreadStarter;
using std::cout;
using std::flush;
using std::endl;

int main()
{
    Channel::Ptr channel1(new Channel);
    Channel::Ptr channel2(new Channel);

    ChannelGroup channels;
    channels.add("1", channel1);
    channels.add("2", channel2);

    Producer producer1(channel1, 1.1, 0.5, TimeSpec(0, TimeSpec::one_second/3));
    Producer producer2(channel2, 0.2, 1.2, TimeSpec(2,TimeSpec::one_second/7));
    Timebase timebase(channels, TimeSpec(3,0));

    JoinableThreadStarter thread1(&producer1);
    JoinableThreadStarter thread2(&producer2);
    JoinableThreadStarter thread3(&timebase);

    thread1.start();
    thread2.start();
    thread3.start();

    ChannelValue v_channel1(channel1);
    ChannelValue v_channel2(channel2);
    PlusValue v_1_plus_2(v_channel1, v_channel2);
    MultValue v_1_plus_2__mult_2(v_1_plus_2, v_channel2);
    ConstValue v_const(7);
    DivValue the_final_value(v_1_plus_2__mult_2, v_const);

    TimeSpec interval(0, TimeSpec::one_second/5);
    for (;;) {
        cout << the_final_value.get() << endl;
        // cout << "1: " << ch1->get() << endl;
        // cout << "2: " << ch2->get() << endl;
        // cout << endl;
        int err = nanosleep(&interval, NULL);
        assert(!err);
    }
}
