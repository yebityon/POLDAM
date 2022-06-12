#pragma once
#include "methodEntry.h"
#include <poldam_util.h>

namespace POLDAM
{
    MethodEntry::MethodEntry(const std::string log_) : log(log_){};

    std::string MethodEntry::getLog()
    {
        return log;
    }

    EventType MethodEntry::getEventType()
    {
        return EventType::METHOD_ENTRY;
    }

    void LogInterpreter<MethodEntry>::parseLog()
    {
        const std::string log = methodEntry.getLog();
        const std::vector<std::string> res = POLDAM_UTIL::parse(log);
    }
}