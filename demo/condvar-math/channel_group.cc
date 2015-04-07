#include "channel_group.h"

ChannelGroup::ChannelGroup() {}

void ChannelGroup::add(
    const std::string& name, 
    const Channel::Ptr& ch)
{
    channels_.push_back(std::make_pair(name, ch));
}
