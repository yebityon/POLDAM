#include "methodEntry.h"

namespace POLDAM
{
    IMethodEntry::IMethodEntry(const std::string log_) : log(log_){};

    std::string IMethodEntry::getLog()
    {
        return log;
    }

    EventType IMethodEntry::getEventType()
    {
        return EventType::METHOD_ENTRY;
    }
}