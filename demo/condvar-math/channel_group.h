#ifndef HAVE_JF_LINUXTOOLS__DEMO__CONDVAR_MATH__CHANNEL_GROUP_H
#define HAVE_JF_LINUXTOOLS__DEMO__CONDVAR_MATH__CHANNEL_GROUP_H

#include "channel.h"

#include <string>
#include <vector>


class ChannelGroup
{
public:
    ChannelGroup();

    void add(
        const std::string& name,
        const Channel::Ptr&);
    size_t nchannels() const { return channels_.size(); }
    Channel::Ptr get(size_t i) const { return channels_[i].second; }

private:
    std::vector<std::pair<std::string, boost::shared_ptr<Channel> > > channels_;
};

#endif
