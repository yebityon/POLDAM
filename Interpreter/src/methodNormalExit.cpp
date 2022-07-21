
#pragma once
#include "methodNormalExit.h"

namespace POLDAM
{
    void LogInterpreter<METHOD_NORMAL_EXIT>::parseLog()
    {
        const std::vector<std::string> parsedLog = POLDAM_UTIL::parse(log);
        METHOD_NORMAL_EXIT rec = {};

        for (unsigned int i = 0; i < parsedLog.size(); ++i)
        {
            std::string str = parsedLog[i];
            const auto &splitByeq = POLDAM_UTIL::split(str, '=');

            switch (i)
            {
            case 0:
                rec.eventId = std::stoul(splitByeq[1]);
                break;
            case 1:
                rec.eventType = POLDAM::EventType::METHOD_NORMAL_EXIT;
                break;
            case 2:
                rec.threadId = std::stoul(splitByeq[1]);
                break;
            case 3:
                rec.dataId = std::stoul(splitByeq[1]);
                break;
            case 4:
                rec.value = std::stoi(splitByeq[1]);
                break;
            case 5:
                // objectType
                break;
            case 6:
                rec.fileAndClassAndMethod = splitByeq[0];
                break;
            default:
                break;
            }

            methodExit = rec;
        }
    }

    std::string LogInterpreter<METHOD_NORMAL_EXIT>::getLog()
    {
        return log;
    }

    EventType LogInterpreter<METHOD_NORMAL_EXIT>::getEventType()
    {
        return EventType::METHOD_NORMAL_EXIT;
    }

    METHOD_NORMAL_EXIT LogInterpreter<METHOD_NORMAL_EXIT>::getParserResult()
    {
        return methodExit;
    }
}